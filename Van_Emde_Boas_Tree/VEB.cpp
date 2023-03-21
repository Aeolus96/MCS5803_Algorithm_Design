// I have neither given nor received any unauthorizedaid in completing this work,
// nor have I presented someone else's work as my own.
// Your Name: Devson Butani
// LTU ID: 000732711
// Date: 12/11/2022


/*
### What is Von Ende Boas Trees vEBT?
# Van Emde Boas tree is an associated array implemented with m-bit integer keys through a 
# multiway tree structure. It performs all operations (insert, delete, lookup, maximum, 
# minimum, successor and predecessor) in O(log log M) time, where M is the maximum number
# of elements that can be stored in the tree.

### How does vEBT work and why it has O(lglgn)?
# vEBT is expected to perform five operations:
# 1) find(k): Find the key k and the value it points to.                            O(log2log2U)
# 2) predecessor(k): Return the largest key in domain less than or equal to k.      O(log2log2U)
# 3) successor(k): Return the smallest key in domain less greater or equal to k.    O(log2log2U)
# 4) insert(k): Insert a key k into the tree.                                       O(log2log2U)
# 5) delete(k): Delete the key k from tree.                                         O(log2log2U)
#
# The vEBT has O(lglgU) due to all its sub operations running at O(lglgU)
#  
# vEB stores the keys in a bounded universe and performs above operations in double logarithmic 
# time complexity with respect to universe size. vEB tree for a universe U containing [0 ... U-1] 
# is implemented by dividing the universe into sqrt(U) vEB trees, where the ith subtree would be 
# responsible for universe of size sqrt(U) containing the keys [(i)sqrt(U) ... (i+1)sqrt(U-1]. The data structure 
# is implemented recursively, where it shrinks by a factor of sqrt(U) at every level.
#
# The vEB tree is a modification of proto-vEB structure and stores following information:
# 1) children array of size sqrt(U) that points to children vEB trees. child i points to vEB tree responsible 
# for the range [(i)sqrt(U) ... (i+1)sqrt(U-1].
# 2) min to store minimum element in the universe of the tree.
# 3) max to store maximum element in the universe of the tree.
# 4) summary, an auxillary vEB tree to keep track of which subtree/child is empty.
# 5) A variable u to keep track of universe size.
#
# Sources: https://iq.opengenus.org/van-emde-boas-tree/

### What is vEBT used for?
# vEB trees were made specefically to solve predecessor problem in double logarithmic time. vEB 
# trees can be used as extremely fast priority queues and find uses in routing applications. If 
# universe size is reasonable, vEB trees are basically faster binary search trees.
# Examples inlcude, LAN network IP message passing and data management systems

### What is preventing vEBT from being used more?
# For small vEB trees, the overhead associated is enormous (on the order of 2^(m/2)) due to the fact
# its complexity is defined not on the size of the set stored but on the size of the universe of values.
# Therefore, the cases in which vEB is useful are extremely limited.

### Implement the algorithm:
*/


#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class vEB
{
    // Elements of each vEB tree node
    int u;
    int min;
    int max;
    vEB *summary;
    vector<vEB *> cluster;

    // Index finders:
    // Subtree index
    int high_idx(int k)
    {
        int x = ceil(sqrt(u));
        return floor(k / x);
    }
    // Index inside subtree
    int low_idx(int k)
    {
        int x = ceil(sqrt(u));
        return k % x;
    }
    // Index from cluster number and position inside cluster
    int index(int k, int kk)
    {
        int x = ceil(sqrt(u));
        return (k * x) + kk;
    }

public:
    // Constructor for empty tree of size U
    vEB(int U_size)
    {
        u = U_size;
        min = -1;
        max = -1;

        // Base size A[0..1]
        if (u <= 2)
        {
            // setup empty base tree
            summary = nullptr;
            cluster = vector<vEB *>(0, nullptr);
        }
        // Recursively setup empty clusters and their bases
        else
        {
            int cluster_count = ceil(sqrt(U_size));
            summary = new vEB(cluster_count);
            cluster = vector<vEB *>(cluster_count, nullptr);
            for (int i = 0; i < cluster_count; i++)
            {
                cluster[i] = new vEB(ceil(sqrt(U_size)));
            }
        }
    }

    // Insert value into the tree
    void insert_val(int k)
    {
        // Empty tree update to one element
        if (min == -1)
        {
            min = k;
            max = k;
        }
        else
        {
            // Swap with min if smaller than min
            if (k < min)
            {
                int temp;
                temp = min;
                min = k;
                k = temp;
            }
            // If not the base-case
            if (u > 2)
            {
                // If empty, update to one element
                if (cluster[high_idx(k)]->min == -1)
                {
                    // Update summary tree
                    summary->insert_val(high_idx(k));
                    // New min max
                    cluster[high_idx(k)]->min = low_idx(k);
                    cluster[high_idx(k)]->max = low_idx(k);
                }
                // If present, insert value recursively
                else
                {
                    cluster[high_idx(k)]->insert_val(low_idx(k));
                }
            }
            // New max if greater than max
            if (k > max)
            {
                max = k;
            }
        }
    }

    // Find the next index with value present in tree. 0 = not found
    int successor(int k)
    {
        // Element in the same base set
        if (u == 2)
        {
            if (k == 0 && max == 1)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        // Return index of min if input is less than min
        else if (min != -1 && k < min)
        {
            return min;
        }
        // Check high in cluster or move to the next one
        else
        {
            // temp store indexes for later use
            int high = high_idx(k);
            int low = low_idx(k);

            // Return index of successor inside this cluster
            if (cluster[high]->max != -1 && low < cluster[high]->max)
            {
                // Find local index
                int local_idx = cluster[high]->successor(low);
                return index(high, local_idx);
            }
            // Move search to the next subtree
            else
            {
                int succ_cluster = summary->successor(high);
                if (succ_cluster == 0)
                {
                    return 0;
                }
                else
                {
                    // Find local index for next tree
                    int local_idx = cluster[succ_cluster]->min;
                    return index(succ_cluster, local_idx);
                }
            }
        }
    }

    // Find the previous index with value present in tree. 0 = not found
    int predecessor(int k)
    {
        // Element in the same base set
        if (u == 2)
        {
            if (k == 1 && min == 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        // Return index of max if input is more than max
        else if (max != -1 && k > max)
        {
            return max;
        }
        // Check high in cluster or move to the next one
        else
        {
            // temp store indexes for later use
            int high = high_idx(k);
            int low = low_idx(k);

            // Return index of predecessor inside this cluster
            if (cluster[high]->min != -1 && low > cluster[high]->min)
            {
                // Find local index
                int local_idx = cluster[high]->predecessor(low);
                return index(high, local_idx);
            }
            // Move search to the previous subtree
            else
            {
                int pred_cluster = summary->predecessor(high);
                if (pred_cluster == 0)
                {
                    if (min != -1 && k > min)
                    {
                        return min;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    // Find local index for previous tree
                    int local_idx = cluster[pred_cluster]->max;
                    return index(pred_cluster, local_idx);
                }
            }
        }
    }

    // Delete a key from the tree
    void delete_val(int k)
    {
        // Base case with 1 key
        if (max == min)
        {
            min = -1;
            max = -1;
        }

        // Case with 2 keys
        else if (u == 2)
        {
            if (k == 0)
            {
                min = 1;
            }
            else
            {
                min = 0;
            }
            max = min;
        }
        else
        {
            // Case with 1st cluster
            if (k == min)
            {
                int first_cluster = summary->min;
                k = index(first_cluster, cluster[first_cluster]->min);
                min = k;
            }

            // temp store indexes for later use
            int high = high_idx(k);
            int low = low_idx(k);

            // Delete key
            cluster[high]->delete_val(low);

            // After deleting the key, fix the summary
            if (cluster[high]->min == -1)
            {
                summary->delete_val(high);
                if (k == max)
                {
                    int summary_max = summary->max;
                    if (summary_max == -1)
                    {
                        max = min;
                    }
                    else
                    {
                        max = index(summary_max, cluster[summary_max]->max);
                    }
                }
            }
            // Replace max with next max
            else if (k == max)
            {
                max = index(high_idx(k), cluster[high_idx(k)]->max);
            }
        }
    }
};
// End of vEB struct

int main()
{
    vEB tree(16); // U = 16
    cout << "Insert {2,3,4,5,7,14,15}\n";
    tree.insert_val(4);
    tree.insert_val(5);
    tree.insert_val(7);
    tree.insert_val(2);
    tree.insert_val(3);
    tree.insert_val(15);
    tree.insert_val(14);
    cout << "Successor of 2: " << tree.successor(2) << '\n';
    cout << "Successor of 8: " << tree.successor(8) << '\n';
    cout << "Successor of 14: " << tree.successor(14) << '\n';
    cout << "Delete {5,15}" << '\n';
    tree.delete_val(5);
    tree.delete_val(15);
    cout << "Predecessor of 7: " << tree.predecessor(7) << '\n';
    cout << "Successor of 14: " << tree.successor(14) << '\n';
    cout << "0, means no successor found!" << '\n';
    return 0;
}