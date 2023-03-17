#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

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

    // Iterate until convergence or maximum number of iterations is reached
    for (int iteration = 0; iteration < max_iterations; ++iteration)
    {
        // Clear the points from each cluster
        for (auto &cluster : clusters)
        {
            cluster.clear_points();
        }

        // Assign each point to the nearest cluster
        for (const auto &point : points)
        {
            double min_distance = std::numeric_limits<double>::max();
            int closest_cluster_index = -1;
            for (int i = 0; i < k; ++i)
            {
                double distance = point.distance(clusters[i].centroid);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    closest_cluster_index = i;
                }
            }
            clusters[closest_cluster_index].add_point(point);
        }

        // Update the centroids of each cluster
        bool converged = true;
        for (auto &cluster : clusters)
        {
            if (cluster.points.empty())
            {
                continue;
            }
            Point old_centroid = cluster.centroid;
            double sum_x = 0, sum_y = 0;
            for (const auto &point : cluster.points)
            {
                sum_x += point.x;
                sum_y += point.y;
            }
            cluster.centroid.x = sum_x / cluster.points.size();
            cluster.centroid.y = sum_y / cluster.points.size();
            if (cluster.centroid.distance(old_centroid) > 1e-6)
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

    for (int i = 0; i < 10000000; ++i)
    {
        points[i] = Point(i, i);
    }

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();

    // Run the K-means clustering algorithm with k=3 and maximum iterations=10
    std::vector<Cluster> clusters = k_means_clustering(points, 4, 10);

    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();

    // Print the time taken 
    auto elapsed_time_sec = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
    std::cout << "Time taken: " << elapsed_time_sec * 1000000 << " microseconds" << std::endl;

    // Print the results
    // for (int i = 0; i < clusters.size(); ++i)
    // {
    //     std::cout << "Cluster " << i << " centroid: (" << clusters[i].centroid.x << ", " << clusters[i].centroid.y << ")" << std::endl;
    //     std::cout << "Points in cluster " << i << ":" << std::endl;
    //     for (const auto &point : clusters[i].points)
    //     {
    //         std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
