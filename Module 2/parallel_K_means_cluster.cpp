#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <omp.h>

// Define the number of threads to use
const int num_threads = 8;

// Point class to represent a 2D point
class Point
{
public:
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    double distance(const Point &other) const
    {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
};

// Cluster class to represent a cluster of points
class Cluster
{
public:
    Point centroid;
    std::vector<Point> points;
    Cluster(Point centroid) : centroid(centroid) {}
    void clear_points()
    {
        points.clear();
    }
    void add_point(const Point &point)
    {
        points.push_back(point);
    }
};

// K-means clustering algorithm
std::vector<Cluster> k_means_clustering(const std::vector<Point> &points, int k, int max_iterations)
{
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, points.size() - 1);

    // Initialize clusters with random centroids
    std::vector<Cluster> clusters;
    for (int i = 0; i < k; ++i)
    {
        clusters.push_back(Cluster(points[dist(gen)]));
    }

    // Set number of OpenMP threads to use
    omp_set_num_threads(num_threads);

    // Iterate until convergence or maximum number of iterations is reached
    for (int iteration = 0; iteration < max_iterations; ++iteration)
    {
        // Clear the points from each cluster
#pragma omp parallel for
        for (auto &cluster : clusters)
        {
            cluster.clear_points();
        }

        // Assign each point to the nearest cluster
#pragma omp parallel for
        for (int i = 0; i < points.size(); ++i)
        {
            double min_distance = std::numeric_limits<double>::max();
            int closest_cluster_index = -1;
            for (int j = 0; j < k; ++j)
            {
                double distance = points[i].distance(clusters[j].centroid);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    closest_cluster_index = j;
                }
            }
#pragma omp critical
            {
                clusters[closest_cluster_index].add_point(points[i]);
            }
        }

        // Update the centroids of each cluster
        bool converged = true;
#pragma omp parallel for reduction(&& \
                                   : converged)
        for (int i = 0; i < k; ++i)
        {
            if (clusters[i].points.empty())
            {
                continue;
            }
            Point old_centroid = clusters[i].centroid;
            double sum_x = 0, sum_y = 0;
            for (const auto &point : clusters[i].points)
            {
                sum_x += point.x;
                sum_y += point.y;
            }
            clusters[i].centroid.x = sum_x / clusters[i].points.size();
            clusters[i].centroid.y = sum_y / clusters[i].points.size();
            if (clusters[i].centroid.distance(old_centroid) > 1e-6)
            {
                converged = false;
            }
        }
        if (converged)
        {
            break;
        }
    }
    return clusters;
}

// Main function to test the K-means clustering algorithm
int main()
{
    // Generate some random points
    std::vector<Point> points(10000000);
#pragma omp parallel for
    for (int i = 0; i < 10000000; ++i)
    {
        points[i] = Point(i, i);
    }

    // Run the K-means clustering algorithm with k=4 and maximum iterations=10
    int k = 4;
    int max_iterations = 10;
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<Cluster> clusters = k_means_clustering(points, k, max_iterations);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto elapsed_time_sec = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
    std::cout << "Time taken: " << elapsed_time_sec * 1000000 << " microseconds" << std::endl;

    // Print the results
    // #pragma omp parallel for
    // for (int i = 0; i < clusters.size(); ++i)
    // {
    //     std::cout << "Cluster " << i << " centroid: (" << clusters[i].centroid.x << ", " << clusters[i].centroid.y << ")" << std::endl;
    //     std::cout << "Points in cluster " << i << ": " << clusters[i].points.size() << std::endl;
    // }

    return 0;
}
