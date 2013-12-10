#ifndef MIN_SEGMENT_TREE_H

#define MIN_SEGMENT_TREE_H

#include <algorithm>
#include <climits>

#include "advanced_segment_tree.h"

/* 
 * Segment tree for the min operation with += update
 */
namespace MinSegmentTreeAdd {

    class UpdInfo {
        public:
            UpdInfo() {
                val = 0;
            }

            explicit UpdInfo(int new_val) {
                val = new_val;
            }

            int getVal() const {
                return val;
            }

            void push(UpdInfo &from, int l, int r) {
                val += from.getVal();
            }

        private:
            int val;
    };

    class TreeNode {
        public:
            TreeNode() {
                val = INT_MAX;
            };

            explicit TreeNode(int new_val) {
                val = new_val;
            }

            int getVal() const {
                return val;
            }

            void merge(TreeNode &left, TreeNode &right) {
                val = std::min(left.getVal(), right.getVal());
            }

            void addUpdate(UpdInfo &a, int l, int r) {
                val += a.getVal(); 
            }

        private:
            int val;
    };
}

typedef SegmentTree<MinSegmentTreeAdd::TreeNode, MinSegmentTreeAdd::UpdInfo>
MinTreeAdd;

/* 
 * Segment tree for the min operation with assignment update
 */
namespace MinSegmentTreeAssignment {

    class UpdInfo {
        public:
            UpdInfo() {
                val = 0;
            }

            explicit UpdInfo(int new_val) {
                val = new_val;
            }

            int getVal() const {
                return val;
            }

            void push(UpdInfo &from, int l, int r) {
                val = from.getVal();
            }

        private:
            int val;
    };

    class TreeNode {
        public:
            TreeNode() {
                val = INT_MAX;
            };

            explicit TreeNode(int new_val) {
                val = new_val;
            }

            int getVal() const {
                return val;
            }

            void merge(TreeNode &left, TreeNode &right) {
                val = std::min(left.getVal(), right.getVal());
            }

            void addUpdate(UpdInfo &a, int l, int r) {
                val = a.getVal(); 
            }

        private:
            int val;
    };
}

typedef SegmentTree<MinSegmentTreeAssignment::TreeNode, 
        MinSegmentTreeAssignment::UpdInfo>
MinTreeAssignment;

/* 
 * Segment tree for the min operation with assignment update and += update
 */
namespace MinSegmentTreeAssignmentAdd {

    class UpdInfo {
        public:
            UpdInfo() {
                assigned = added = 0;
                is_assigned = false;
            }

            UpdInfo(int new_assigned, int new_added, bool new_is_assigned) {
                is_assigned = new_is_assigned;
                assigned = new_assigned;
                added = new_added;
            }

            int getAdded() const {
                return added;
            }

            int getAssigned() const {
                return assigned;
            }

            bool isAssigned() const {
                return is_assigned;
            }

            void push(UpdInfo &from, int l, int r) {
                if (from.isAssigned()) {
                    assigned = from.getAssigned();
                    added = from.getAdded();
                    is_assigned = from.isAssigned();
                } else {
                    added += from.added;
                }
            }

        private:
            int assigned, added;
            bool is_assigned;
    };

    class TreeNode {
        public:
            TreeNode() {
                val = INT_MAX;
            };

            explicit TreeNode(int new_val) {
                val = new_val;
            }

            int getVal() const {
                return val;
            }

            void merge(TreeNode &left, TreeNode &right) {
                val = std::min(left.getVal(), right.getVal());
            }

            void addUpdate(UpdInfo &a, int l, int r) {
                if (a.isAssigned()) {
                    val = a.getAssigned() + a.getAdded();
                } else {
                    val += a.getAdded();
                }
            }

        private:
            int val;
    };
}

typedef SegmentTree<MinSegmentTreeAssignmentAdd::TreeNode, 
        MinSegmentTreeAssignmentAdd::UpdInfo>
MinTreeAssignmentAdd;

#endif
