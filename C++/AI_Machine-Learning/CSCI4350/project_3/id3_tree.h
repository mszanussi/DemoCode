/*
 *    Programmer: Milan Zanussi
 *    Course: Introduction to Artificial Intelligence (CSCI 4350)
 *    Professor: Dr. Joshua Phillips
 *
 *    Project: The ID3 Algorithms for Supervised Learning
 *    Date: November 16th, 2018
 *    File: id3_tree.h
 *    File Description: Header file containing the definition
 *    of a class which takes in a continuous data set (arrange with the
 *    rows representing the attributes and the columns representing
 *    the data samples) and constructs an ID3 decision tree from the data.
 *
 */

#ifndef ID3TREE
#define ID3TREE

#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>

// Attribute sorting function provided by Dr. Phillips
std::vector<std::vector<int> > sort_attributes(std::vector<std::vector<double> > data) 
{
  std::vector<std::vector<int> > indices;
  std::vector<double> *ptr;
  indices.resize(data.size());
  for (int x = 0; x < indices.size(); x++) {
    indices[x].resize(data[x].size());
    std::iota(indices[x].begin(),indices[x].end(),0);
    ptr = &(data[x]);
    std::sort(indices[x].begin(),indices[x].end(),
	 [&](size_t i, size_t j){ return (*ptr)[i] < (*ptr)[j]; });
  }
  return indices;
}

// id3_tree: A class representing the decision structure created from
// a training data set which can be used as a classifier.
class id3_tree
{

    // ******************** PUBLIC CLASS CONTENTS ******************** \\

    public:

        // id3_tree constructor: Takes a training set (assumed complete) with a
        // classification vector representing the classification for each
        // corresponding row in the training data matrix.
        id3_tree(std::vector<std::vector<double>> data_matrix, std::vector<int> classifications)
        {
            //std::cout << "Generating Tree..." << std::endl;

            // Number of predictors is the field count
            if(!data_matrix.empty())
                predictors = data_matrix.size();

            // Create vector of possible attribute values
            for(int i = 0; i < classifications.size(); ++i)
            {
                // If the category value is new, then add it to the list of category values
                if(std::find(category_values.begin(), category_values.end(), classifications[i]) == category_values.end())
                    category_values.push_back(classifications[i]);
            }

            // Recursively generate the ID3 tree
            root = GenerateNode(data_matrix, classifications);
            //std::cout << "Tree successfully generated!" << std::endl;
            return;            
        }
		
        // Lookup: Takes a piece of data and attempts to classify the data
        // based on the information provided.
        int Classify(std::vector<double> data)
        {
            // If the input data has more fields than the number of
            // predictors used to construct the tree, there is an error
            if(predictors != data.size())
            {
                std::cerr << "ERROR: Data does not match number of fields." << std::endl;
                return -1;
            }
		
            // Return result of recursive lookup
            return Classify(data, root);
        }

        // id3_tree desctructor: Cleans up memory allocated to constructing
        // the id3_tree.
        ~id3_tree()
        {
            // Recursively destroy tree
            Destroy(root);

            // Let other members be destroyed by their destructors
            return;
        }

    // ******************** PRIVATE CLASS CONTENTS ******************** \\

    private:

        // Defines nodes for the id3_tree
        struct Node
        {
            int attribute;           // Attribute label or, if the node is terminal, the class number 
            double split_point;      // Splitting point for continuously valued attributes
            Node* less;              // Class of all objects with attribute less than split
            Node* greater;           // Class of all objects with attributes greater than split
        };
		

        Node* root;                  // Root node of tree
        std::vector<int> category_values; // Classes into which objects can be classified
        unsigned int predictors;     // Number of predictors associated with each entry in tree

        // GenerateNode: Private function for generating nodes using the ID3 algorithm
        // (field_count does not need to be passed to this function since all entries
        //  in local_data are assumed to be the same length as "predictors" variable).
        Node* GenerateNode(std::vector<std::vector<double>> local_data, std::vector<int> local_classes)
        {
            // Create a new node to store node information 
            Node* new_node = new Node;

            //std::cout << "Counting occurrences of each category..." << std::endl;
            //std::cout << "Size of local data: " << local_data[0].size() << std::endl;
            // Calculate the total number of objects in each category
            std::vector<unsigned int> total_counts;
            for( std::vector<int>::iterator category = category_values.begin(); category != category_values.end(); ++category )
            {
                unsigned int count = 0;

                // Count number of occurences of a category value
                for (std::vector<int>::iterator entry = local_classes.begin(); entry != local_classes.end(); ++entry )
                    if( *category == *entry )
                        ++count;

                // If all objects are of the same category, the data can be trivially classified
                if ( count == local_classes.size() )
                {
                    // Create terminal node of the category
                    new_node->attribute = *category;
                    new_node->less = nullptr;
                    new_node->greater = nullptr;

                    // Return node
                    return new_node;
                }

                //std::cout << "Count of class " << *category << " is " << count << std::endl;

                // Push probability to the vector
                total_counts.push_back(count);
            }

            // Calculate the total information in the class divisions
            double total_information = 0.0;
            for(int i = 0; i < total_counts.size(); ++i)
                if( total_counts[i]/double(local_data[0].size()) > 0 )
                    total_information += -(total_counts[i]/double(local_data[0].size()))*log2(total_counts[i]/double(local_data[0].size()));


            //std::cout << "Total Information: " << total_information << std::endl;

            //std::cout << "Sorting Attributes..." << std::endl;
            // Sort the local data set with respect to each attribute and store
            // the orderings in a new matrix
            std::vector<std::vector<int>> sorted_index;
            sorted_index = sort_attributes(local_data);

            //std::cout << "Finding maximal split..." << std::endl;
            // Find split point of maximal information gain initialized with
            // information gain equal to -1 to indicate no entry exists yet.
            std::pair<unsigned int,std::pair<double,double>> max_info_split;
            max_info_split.second.second = -1.0;
            for(unsigned int i = 0; i < predictors; ++i)    // This loop iterates over attributes
            {

                // Initialize a count of objects classified as less than a split point
                std::vector<unsigned int> less_counts;
                for(unsigned int c = 0; c < category_values.size(); ++c)
                    less_counts.push_back(0);

                // Find maximum information for each split point for the given attribute
                double prev_val;
                for(unsigned int k = 0; k < local_data[0].size(); ++k)    // Iterates through each element in a column
                {

                    // Cannot create split until first element has been parsed
                    if( k == 0 )
                    {
                        prev_val = local_data[i][sorted_index[i][k]];
                    }

                    // Check for a gap, if one exists compute the information
                    else if( prev_val < local_data[i][sorted_index[i][k]] )
                    {
                        // Information expected from the 
                        double exp_information = 0.0;

                        // Calculate the information using the counting data
                        for(unsigned int m = 0; m < less_counts.size(); ++m)
                        {
                            //std::cout << local_data.size() << " " << total_counts[m] << std::endl;
                            //std::cout << k << " " << less_counts[m] << std::endl;
                            // p( less | class ) = less_counts[m]/double(k)
                            // p( greater | class ) = double(total_counts[m] - less_counts[m])/(local_data[i].size() - k)
                            // p( less ) = double(k)/local_data[i].size()
                            // p( greater ) = double(local_data[i].size() - k)/local_data[i].size()
                            //std::cout << double(less_counts[m])/double(k) << std::endl;
                            if(double(less_counts[m])/double(k) > 0.0)
                                exp_information -= double(k)/double(local_data[0].size()) * double(less_counts[m])/double(k) * log2(double(less_counts[m])/double(k));

                            //std::cout << log2(double(total_counts[m] - less_counts[m])/double(local_data.size() - k)) << std::endl;
                            if(double(total_counts[m] - less_counts[m])/double(local_data[0].size() - k) > 0.0)
                                exp_information -= (double(local_data[0].size() - k)/double(local_data[0].size()))
                                                    *(double(total_counts[m] - less_counts[m])/double(local_data[0].size() - k))
                                                    *log2(double(total_counts[m] - less_counts[m])/double(local_data[0].size() - k));
                        }

                        //std::cout << "Expected information: " << exp_information << std::endl;

                        // Replace the currently known best informaton split if need be
                        if( (max_info_split.second.second < total_information - exp_information) ||
                            ((max_info_split.second.second == total_information - exp_information) &&  
                                (max_info_split.first > i || (max_info_split.first == i && max_info_split.second.first > (prev_val + local_data[i][sorted_index[i][k]])/2.0))) )
                        {
                            // Replace currently known best
                            max_info_split.first = i;
                            max_info_split.second.first = (prev_val + local_data[i][sorted_index[i][k]])/2.0;
                            max_info_split.second.second = total_information - exp_information;
                        }

                    }

                    // Classify current piece of data and add it to the sum of lesser elements
                    for(unsigned int categ = 0; categ < less_counts.size(); ++categ)
                        if(category_values[categ] == local_classes[sorted_index[i][k]])
                            ++less_counts[categ];
                                

                    // Let the previous value be equal to the current value
                    prev_val = local_data[i][sorted_index[i][k]];

                }
                
            }

            // Calculate lesser set and greater set
            std::vector<std::vector<double>> less_data, greater_data;
            std::vector<int> less_classes, greater_classes;

            //std::cout << "Partitioning data about split..." << std::endl;
            //std::cout << max_info_split.first << " " << max_info_split.second.first << " " << max_info_split.second.second << std::endl;

            std::vector<unsigned int> less_indices, greater_indices;
            // Scan local_data and separate them into the lesser and greater data
            for(unsigned int t = 0; t < local_data[0].size(); ++t)
            {
                if( local_data[max_info_split.first][t] < max_info_split.second.first )
                {
                    less_indices.push_back(t);
                    less_classes.push_back(local_classes[t]);
                }

                else
                {
                    greater_indices.push_back(t);
                    greater_classes.push_back(local_classes[t]);
                }
            }

            // For each attribute
            for(unsigned int attr = 0; attr < local_data.size(); ++attr)
            {

                std::vector<double> new_row;

                // Add lesser-indexed data to less sets
                for(std::vector<unsigned int>::iterator lit = less_indices.begin(); lit < less_indices.end(); ++lit)
                {
                    new_row.push_back(local_data[attr][*lit]);
                }

                less_data.push_back(new_row);
                new_row.clear();

                // Add greater-indexed data to greater sets
                for(std::vector<unsigned int>::iterator grit = greater_indices.begin(); grit < greater_indices.end(); ++grit)
                {
                    new_row.push_back(local_data[attr][*grit]);
                }

                greater_data.push_back(new_row);
                new_row.clear();

            }

            // Calculate less node
            //std::cout << "Number of lesser objects: " << less_data.size() << std::endl;
            new_node->less = GenerateNode(less_data, less_classes);

            // Calculate greater node
            //std::cout << "Number of greater objects: " << greater_data.size() << std::endl;
            new_node->greater = GenerateNode(greater_data, greater_classes);

            // Assign split point
            new_node->split_point = max_info_split.second.first;

            // Assign attribute
            new_node->attribute = max_info_split.first;

            // Return node
            return new_node;
        }
	
        // Classify: Private lookup function for recursively classifying a data entry
        // (the length of data is assumed to be equal to the "predictors" variable).
        int Classify(std::vector<double> data, Node* curr_node)
        {
            //std::cout << "classifying" << std::endl;
            // If less == nullptr, then greater == nullptr by construction
            if(curr_node->less != nullptr)
            {
                // If data < split, then the object is classified in the less branch
                if(data[curr_node->attribute] < curr_node->split_point)
                {   /* std::cout << "less" << std::endl;*/ return Classify(data, curr_node->less); }

                // Otherwise, the data must be in the greater half
                else
                {    /*std::cout << "greater" << std::endl;*/ return Classify(data, curr_node->greater);    }
            }

            // Otherwise, return attribute since the node is terminal
            return curr_node->attribute;
        }
	
        // Destroy: Recursively destroys nodes for cleaning memory during destruction
        void Destroy(Node* parent)
        {
            // If the children are not null pointers, destroy them
            if(parent->less != nullptr)
            {
                Destroy(parent->less);
                Destroy(parent->greater);
            }

            // Destroy local node
            delete parent;

            return;
        }

    // ******************** END OF CLASS DEFINITION ******************** \\
	
};

#endif
