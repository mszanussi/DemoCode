
#ifndef KMEANS
#define KMEANS

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class kmeans
{

    public:

        // CONSTRUCTOR : Builds a K-Means classifier using the input data set inputs and 
        // their corresponding classifications
        kmeans(int rand_seed, unsigned int cluster_input, unsigned int feature_input, std::vector<std::vector<double>> data_set, std::vector<int> classifications, unsigned int Lp = 2)
            : clusters(cluster_input), features(feature_input), p_norm(Lp)
        {

            // If the class and data sizes mismatch, then the kmeans cannot be performed
            if( classifications.size() != data_set.size() )
            {
                std::cerr << "ERROR,KMEANS: Data points have a mismatched size to class vector." << std::endl;
                exit(-1);
            }

            // Create vector of possible attribute values
            //std::cout << "Identifying classes" << std::endl;
            for(int d = 0; d < classifications.size(); ++d)
            {
                // If the category value is new, then add it to the list of category values
                if(std::find(classes.begin(), classes.end(), classifications[d]) == classes.end())
                    classes.push_back(classifications[d]);
            }

            // Randomly select some data points to be the initial centroids
            //std::cout << "Seeding RNG" << std::endl;
            srand(rand_seed);

            //std::cout << "Initializing centroids" << std::endl;
            for(unsigned int i = 0; i < clusters; ++i)
            {
                // Choose random indexed data point
                unsigned int index = rand()%(data_set.size());

                // Create new centroids and insert it into set of centroids
                std::pair<std::vector<double>,int> new_centroid;
                new_centroid.first = data_set[index];
                new_centroid.second = classifications[index];
                centroids.push_back(new_centroid);
            }            

            // Create array of which data points are assigned to which cluster
            std::vector<unsigned int> cluster_assignments;
            cluster_assignments.resize(data_set.size());

            // Begin K-Means iterations
            //std::cout << "Beginning K-means iteration" << std::endl;
            bool centroids_moved = true;
            while(centroids_moved)
            {
                // Calculate distance of each point from each centroid
                //std::cout << "\tCalculating distances" << std::endl;
                for(unsigned int j = 0; j < data_set.size(); ++j)
                {
                    // Stores cluster with implicit distance
                    std::pair<unsigned int, double> cluster;
                    cluster.second = -1.0;

                    // Iterate through each cluster to find the closest one
                    for(unsigned int k = 0; k < clusters; ++k)
                    {

                        //std::cout << "\t\tLpDist : " << LpNorm(data_set[j], centroids[k].first) << std::endl;

                        // If we find a closer cluster, we assign the cluster to the new one
                        if(cluster.second > LpNorm(data_set[j], centroids[k].first) || cluster.second < 0.0)
                        {
                            cluster.first = k;
                            cluster.second = LpNorm(data_set[j],centroids[k].first);
                        }
                    }

                    // Assign the data point at index j with the found closest cluster
                    cluster_assignments[j] = cluster.first;
                }

                // Generate new centroids by averaging its assigned data points
                // We begin by initializing a vector of cluster sizes to all zeros
                //std::cout << "\tTaking avarages" << std::endl;
                std::map<unsigned int,unsigned int> cluster_sizes;
                for(unsigned int c = 0; c < clusters; ++c) 
                {
                    // Key is the cluster, value is the number of items associated with the cluster
                    std::pair<unsigned int, unsigned int> cluster_size;
                    cluster_size.first = c;
                    cluster_size.second = 0;
                    cluster_sizes.insert(cluster_size);
                }

                // Sum clusters and count the number of points in each cluster
                //std::cout << "\t\tTaking sum" << std::endl;
                std::vector<std::pair<std::vector<double>,int>> new_centroids = centroids;
                for(unsigned int p = 0; p < data_set.size(); ++p)
                {
                    // If this is the first point in the average, assign the sum
                    // to this value.
                    if(cluster_sizes[cluster_assignments[p]] == 0)
                    {
                        /*std::cout << "\t\t\t";
                        for(unsigned int a = 0; a < data_set[p].size(); ++a) std::cout << data_set[p][a] << " ";
                        std::cout << "is associated with cluster " << cluster_assignments[p] << std::endl;*/

                        new_centroids[cluster_assignments[p]].first = data_set[p];
                        cluster_sizes[cluster_assignments[p]] = 1;
                    }

                    // Otherwise, simply add the point to the sum
                    else
                    {
                        for(unsigned int q = 0; q < features; ++q)
                        {
                            new_centroids[cluster_assignments[p]].first[q] += data_set[p][q];
                        }
                        cluster_sizes[cluster_assignments[p]] += 1;

                        /*std::cout << "\t\t\t";
                        for(unsigned int a = 0; a < data_set[p].size(); ++a) std::cout << data_set[p][a] << " ";
                        std::cout << "is associated with cluster " << cluster_assignments[p] << std::endl;*/
                    }
                }

                // For each nonempty cluster, divide each component by the number of points in each cluster
                // (if the cluster is empty, leave the point fixed ie do nothing)
                //std::cout << "\t\tDividing sums" << std::endl;
                for(unsigned int m = 0; m < clusters; ++m)
                {
                    /*std::cout << "Sum vector of cluster " << m << " : ";
                    for(unsigned int g = 0; g < features; ++g) std::cout << new_centroids[m].first[g] << " ";
                    std::cout << std::endl;*/

                    if(cluster_sizes[m] != 0)
                    {
                        // Divide each component by the number of entries associated with the cluster
                        for(unsigned int n = 0; n < features; ++n)
                            new_centroids[m].first[n] /= double(cluster_sizes[m]);
                    }

                    /*std::cout << "Average vector of cluster " << m << " : ";
                    for(unsigned int g = 0; g < features; ++g) std::cout << new_centroids[m].first[g] << " ";
                    std::cout << std::endl;*/
                }

                // Assign new centroids to old centroids
                //std::cout << "\tChecking for movement" << std::endl;
                centroids_moved = false;
                for(unsigned int t = 0; t < clusters; ++t)
                {
                    // If any centroids changes, designate so
                    if(new_centroids[t].first != centroids[t].first)
                        centroids_moved = true;

                    centroids[t].first = new_centroids[t].first;
                }

            }

            // Assign each centroid a class label by polling data points assigned to it
            //std::cout << "Assigning class labels" << std::endl;
            for(unsigned int u = 0; u < clusters; ++u)
            {
                // Create vector to count each class
                std::vector<std::pair<int,unsigned int>> class_counts;
                for(unsigned int v = 0; v < classes.size(); ++v)
                {
                    std::pair<int,unsigned int> class_node;
                    class_node.first = classes[v];
                    class_node.second = 0;
                    class_counts.push_back(class_node);
                }

                // Count number of points in each class associated with this cluster
                for(unsigned int w = 0; w < cluster_assignments.size(); ++w)
                {
                    // If the point is associated with the current cluster, add
                    // one to the size of the class it falls under for the cluster.
                    if(cluster_assignments[w] == u)
                    {
                        for(unsigned int h = 0; h < class_counts.size(); ++h)
                        {
                            if(class_counts[h].first == classifications[w])
                            {
                                ++class_counts[h].second;
                                break;
                            }
                        }
                    }
                }

                // Find maximum counted class
                // If no points were assigned to the cluster, pick a random class
                std::pair<int,unsigned int> curr_max;
                curr_max.first = classes[rand()%(classes.size())];
                curr_max.second = 0;
                for(unsigned int x = 0; x < class_counts.size(); ++x)
                {
                    if(class_counts[x].second > curr_max.second)
                    {
                        curr_max.first = class_counts[x].first;
                        curr_max.second = class_counts[x].second;
                    }
                }

                // Assign class label to node
                centroids[u].second = curr_max.first;
            }

            // End of k-means process
            //std::cout << "Classifier constructed" << std::endl;

        }

        // Classify : Classifies a vector of doubles matching the number of features to
        // one of the possible classes by finding the centroid to which it's closest
        int Classify( std::vector<double> data_point )
        {
            std::pair<int, double> classification;
            classification.second = -1.0;

            // Find the centroid to which the data point has shortest distance
            for(unsigned int ell = 0; ell < centroids.size(); ++ell)
            {
                if(LpNorm(centroids[ell].first, data_point) < classification.second || classification.second == -1.0)
                {
                    classification.first = centroids[ell].second;
                    classification.second = LpNorm(centroids[ell].first, data_point);
                }
            }

            // Return class of nearest centroid
            return classification.first;
        }

        // LpNorm : Takes the Lp-norm associated with the supplied p_norm
        // variable used to construct the class. p = 2 corresponds to
        // Euclidean distance, and p = 0 is used to denote the L infinity
        // norm.
        double LpNorm( std::vector<double> x, std::vector<double> y )
        {
            double norm = 0.0;

            // Check to make sure both data points have the same number of entries
            if(x.size() != y.size())
                return -1.0;        // This value denotes an error

            for(unsigned int comp = 0; comp < x.size(); ++comp)
            {
                double comp_diff = std::abs(x[comp] - y[comp]);

                // L-infinity norm
                if(p_norm == 0)
                {    if( norm < comp_diff ) norm = comp_diff;    }

                // Any finite Lp norm
                else
                {    norm += std::pow(comp_diff, double(p_norm));    }
            }

            // L-infinity case
            if(p_norm == 0)
                return norm;

            // Any finite Lp case
            else
                return std::pow(norm, 1.0/double(p_norm));
        }

    private:

        // PRIVATE DATA
        std::vector<std::pair<std::vector<double>,int>> centroids;
        std::vector<int> classes;
        unsigned int clusters;
        unsigned int features;
        unsigned int p_norm;

        /*// LpNorm : Takes the Lp-norm associated with the supplied p_norm
        // variable used to construct the class. p = 2 corresponds to
        // Euclidean distance, and p = 0 is used to denote the L infinity
        // norm.
        double LpNorm( std::vector<double> x, std::vector<double> y)
        {
            double norm = 0.0;

            // Check to make sure both data points have the same number of entries
            if(x.size() != y.size())
                return -1.0;        // This value denotes an error

            for(int comp = 0; comp < x.size(); ++comp)
            {
                double comp_diff = abs(x[comp] - y[comp]);

                // L-infinity norm
                if(p_norm == 0)
                    if( norm < comp_diff ) norm = comp_diff;

                // Any finite Lp norm
                else
                    norm += pow(comp_diff, p_norm);
            }

            // L-infinity case
            if(p_norm == 0)
                return norm;

            // Any finite Lp case
            else
                return pow(norm, 1.0/double(p_norm));

        }*/

};

#endif
