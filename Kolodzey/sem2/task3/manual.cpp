#include "gtest/gtest.h"
#include "graph.h"

using graph::BaseIncidence;
using graph::BaseIterator;
using graph::AdjacencyMatrixIncidence;
using graph::AdjacencyMatrixIterator;
using graph::GraphIterator;
using graph::Graph;

using graph::AccessGraphIterator;
using graph::AccessAdjacencyMatrixIterator;
using graph::AccessAdjacencyMatrixIncidence;

using std::vector;
using std::unique_ptr;

class graph::AccessAdjacencyMatrixIterator {
 public:
   AccessAdjacencyMatrixIterator(AdjacencyMatrixIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<bool>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<bool>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyMatrixIterator* it_ptr_;
};

class graph::AccessAdjacencyMatrixIncidence {
 public:
  AccessAdjacencyMatrixIncidence(AdjacencyMatrixIncidence* li_ptr)
      : li_ptr_(li_ptr) {}
  vector<bool>& getVectorBool() { return li_ptr_->adjdata_; }
  AdjacencyMatrixIncidence* li_ptr_;
};

class graph::AccessGraphIterator {
 public:
  AccessGraphIterator(GraphIterator* it_ptr) : it_ptr_(it_ptr) {}
  vector<unique_ptr<BaseIncidence>>::const_iterator getPos() {
    return it_ptr_->pos_;
  }
  vector<unique_ptr<BaseIncidence>>::const_iterator getEnd() {
    return it_ptr_->end_;
  }
  GraphIterator* it_ptr_;
};

TEST(AdjacencyMatrixIterator, Constructor) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};

  AdjacencyMatrixIterator it(0, values.begin(), values.end());
  
  AccessAdjacencyMatrixIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(0, it.getCurrentVertexId());
}


TEST(AdjacencyMatrixIterator, getCurrentVertexId) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};
  
  AdjacencyMatrixIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.getCurrentVertexId());
  AdjacencyMatrixIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.getCurrentVertexId());
}

TEST(AdjacencyMatrixIterator, isValid) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};
  
  AdjacencyMatrixIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(true, it0.isValid());
  AdjacencyMatrixIterator it3(3, values.end(), values.end());
  EXPECT_EQ(false, it3.isValid());
}

TEST(AdjacencyMatrixIterator, moveForvard) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};  
  AdjacencyMatrixIterator matrix_it(2, values.begin() + 2, values.end());
  AccessAdjacencyMatrixIterator acc_matrix_it(&matrix_it);
  int val = 0;

  for (vector<bool>::iterator it = values.begin(); it != values.end(); ++it) {
    if (*it == true) {
      EXPECT_EQ(val, matrix_it.getCurrentVertexId());
      EXPECT_EQ(it, acc_matrix_it.getPos());
      EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
      matrix_it.moveForvard();
    }
    ++val;
  }
  EXPECT_EQ(-1, matrix_it.getCurrentVertexId());
  EXPECT_EQ(values.end(), acc_matrix_it.getPos());
  EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
  matrix_it.moveForvard();
  EXPECT_EQ(-1, matrix_it.getCurrentVertexId());
  EXPECT_EQ(values.end(), acc_matrix_it.getPos());
  EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
}

TEST(AdjacencyMatrixIncidence, Constructor) {
  bool init[6] = {0, 0, 1, 1, 0, 1};
  vector<bool> values(init, init + 6);
  //Constructor which copies vector<bool>
  AdjacencyMatrixIncidence li1(values);
  AccessAdjacencyMatrixIncidence access_li(&li1);
  EXPECT_EQ(vector<bool>(init, init + 6), access_li.getVectorBool());
  EXPECT_EQ(vector<bool>(init, init + 6), values);

  //Constructor which moves vector<bool>
  AdjacencyMatrixIncidence li2(std::move(values));
  access_li.li_ptr_ = &li2;
  EXPECT_EQ(vector<bool>(init, init + 6), access_li.getVectorBool());
  EXPECT_EQ(vector<bool>(), values);
}

TEST(AdjacencyMatrixIncidence, Begin) {
  bool init[6] = {0, 0, 1, 1, 0, 1};

  AdjacencyMatrixIncidence li1(vector<bool>(init, init + 6));
  std::unique_ptr<BaseIterator> ptr_it1 = li1.begin();
  EXPECT_EQ(2, ptr_it1->getCurrentVertexId());
  AdjacencyMatrixIncidence li2(vector<bool>(init + 2, init + 6));
  std::unique_ptr<BaseIterator> ptr_it2 = li2.begin();
  EXPECT_EQ(0, ptr_it2->getCurrentVertexId());
}

TEST(GraphIterator, Constructor) {
  vector<unique_ptr<BaseIncidence>> values(10);
  GraphIterator it(0, values.begin(), values.end());
  AccessGraphIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(0, it.getCurrentVertexId());
}

TEST(GraphIterator, getCurrentVertexId) {
  vector<unique_ptr<BaseIncidence>> values(10);
  
  GraphIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.getCurrentVertexId());
  GraphIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.getCurrentVertexId());
}

TEST(GraphIterator, isValid) {
  vector<unique_ptr<BaseIncidence>> values(10);
  
  GraphIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(true, it0.isValid());
  GraphIterator it3(3, values.end(), values.end());
  EXPECT_EQ(false, it3.isValid());
}

TEST(GraphIterator, moveForvard) {
  vector<unique_ptr<BaseIncidence>> values(10); 
  GraphIterator it(0, values.begin(), values.end());
  AccessGraphIterator acc_it(&it);
  vector<unique_ptr<BaseIncidence>>::const_iterator v_it = values.begin();
  for (int i = 0; i < 10; ++i, ++v_it) {
    EXPECT_EQ(i, it.getCurrentVertexId());
    EXPECT_EQ(v_it, acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(true, it.isValid());
    it.moveForvard();
  }
    EXPECT_EQ(-1, it.getCurrentVertexId());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
    it.moveForvard();
    EXPECT_EQ(-1, it.getCurrentVertexId());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
}

TEST(Graph, isConnected) {
  bool mval [4][4] = {{0, 1, 0, 0},
                      {1, 0, 0, 1},
                      {1, 1, 0, 1},
                      {0, 0, 1, 0}};
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 4; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 4)));
  Graph graph(std::move(vval));
  for(int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_EQ(mval[i][j], graph.isConnected(i, j));
    }
  }
  EXPECT_DEATH({ graph.isConnected(5, 0); }, "");
  EXPECT_DEATH({ graph.isConnected(-1, 0); }, "");
  EXPECT_DEATH({ graph.isConnected(1, 5); }, "");
  EXPECT_DEATH({ graph.isConnected(2, -1); }, "");
}

TEST(Graph, Begin) {
  bool mval [4][4] = {{0, 1, 0, 0},
                      {1, 0, 0, 1},
                      {1, 1, 0, 1},
                      {0, 0, 1, 0}};
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 4; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 4)));
  Graph graph(std::move(vval));
  for(int i = 0; i < 4; ++i) {
    unique_ptr<BaseIterator> it = graph.begin(i);
    for(int j = 0; j < 4; ++j)
      if (mval[i][j]) {
        EXPECT_EQ(j, it->getCurrentVertexId());
        it->moveForvard();
      }
  }
  unique_ptr<BaseIterator> it = graph.begin(-1);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(i, it->getCurrentVertexId());
    it->moveForvard();
  }
  EXPECT_DEATH({ graph.begin(5); }, "");
  EXPECT_DEATH({ graph.begin(-2); }, "");
}