#include <tuple>
#include "gtest/gtest.h"
#include "graph.h"

using graph::BaseIncidence;
using graph::BaseIterator;
using graph::AdjacencyMatrixIncidence;
using graph::AdjacencyMatrixIterator;
using graph::GraphIterator;
using graph::Graph;
using graph::Coloring;

using graph::getSource;
using graph::getSink;
using graph::getIsolated;
using graph::hasSelfLoop;

using graph::buildSimpleAdjacencyMatrix;

using graph::AccessGraphIterator;
using graph::AccessAdjacencyMatrixIterator;
using graph::AccessAdjacencyMatrixIncidence;

using std::vector;
using std::unique_ptr;
using std::pair;
using std::tuple;
using std::cout;
using std::endl;

using ::testing::Values;
using ::testing::Range;
using ::testing::Combine;

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
  EXPECT_EQ(0, it.get());
}


TEST(AdjacencyMatrixIterator, get) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};
  
  AdjacencyMatrixIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.get());
  AdjacencyMatrixIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.get());
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
      EXPECT_EQ(val, matrix_it.get());
      EXPECT_EQ(it, acc_matrix_it.getPos());
      EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
      matrix_it.moveForvard();
    }
    ++val;
  }
  EXPECT_EQ(-1, matrix_it.get());
  EXPECT_EQ(values.end(), acc_matrix_it.getPos());
  EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
  matrix_it.moveForvard();
  EXPECT_EQ(-1, matrix_it.get());
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
  EXPECT_EQ(2, ptr_it1->get());
  AdjacencyMatrixIncidence li2(vector<bool>(init + 2, init + 6));
  std::unique_ptr<BaseIterator> ptr_it2 = li2.begin();
  EXPECT_EQ(0, ptr_it2->get());
}

TEST(GraphIterator, Constructor) {
  vector<unique_ptr<BaseIncidence>> values(10);
  GraphIterator it(0, values.begin(), values.end());
  AccessGraphIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(0, it.get());
}

TEST(GraphIterator, get) {
  vector<unique_ptr<BaseIncidence>> values(10);
  
  GraphIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.get());
  GraphIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.get());
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
    EXPECT_EQ(i, it.get());
    EXPECT_EQ(v_it, acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(true, it.isValid());
    it.moveForvard();
  }
    EXPECT_EQ(-1, it.get());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
    it.moveForvard();
    EXPECT_EQ(-1, it.get());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
}

TEST(Graph, isConnected) {
  vector<vector<bool>> mval = {{0, 1, 0, 0},
                               {1, 0, 0, 1},
                               {1, 1, 0, 1},
                               {0, 0, 1, 0}};
  Graph graph = buildSimpleAdjacencyMatrix(mval);
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
  vector<vector<bool>> mval = {{0, 1, 0, 0},
                               {1, 0, 0, 1},
                               {1, 1, 0, 1},
                               {0, 0, 1, 0}};
  Graph graph(buildSimpleAdjacencyMatrix(mval));
  for(int i = 0; i < 4; ++i) {
    unique_ptr<BaseIterator> it = graph.begin(i);
    for(int j = 0; j < 4; ++j)
      if (mval[i][j]) {
        EXPECT_EQ(j, it->get());
        it->moveForvard();
      }
  }
  unique_ptr<BaseIterator> it = graph.begin(-1);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(i, it->get());
    it->moveForvard();
  }
  EXPECT_DEATH({ graph.begin(5); }, "");
  EXPECT_DEATH({ graph.begin(-2); }, "");
}

TEST(func, isPath) {
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 1},      //0->1
                                           {0, 0}})); 
  EXPECT_TRUE(isPath(graph2, 0, 1));
  EXPECT_FALSE(isPath(graph2, 1, 0));

  Graph graph4(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},      // 0-->1
                                           {0, 0, 1, 0},      // ^   |
                                           {0, 0, 0, 1},      // |   v  
                                           {1, 0, 0, 0}}));   // 3<--2
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_TRUE(isPath(graph4, i, j));
}

TEST(func, getStronglyConnectedComponentsDummy) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 1, 0, 0, 0},
     /* 0 <-- 2 <-    */                  {0, 0, 0, 0, 0},
     /* |     |   \   */                  {1, 0, 0, 1, 0},
     /* v     v    \  */                  {0, 1, 0, 0, 1},
     /* 1 <-- 3 --> 4 */                  {0, 0, 1, 0, 0}}));  
  Coloring components = getStronglyConnectedComponentsDummy(graph);
  //expected division into components: 0; 1; 2 + 3 + 4 
  EXPECT_EQ(3, components.getNumberOfColors());
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(3));
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(4));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(1));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(0));
  EXPECT_FALSE(components.getColorOf(1) == components.getColorOf(0));
}

TEST(func, getSource) {
Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                        {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
   /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
   /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* v     |           vv v  */
   /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
   /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
   /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
   /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> source = getSource(graph);
  sort(source.begin(), source.end());
  EXPECT_EQ(2, source.size());
  EXPECT_EQ(5, source[0]);
  EXPECT_EQ(6, source[1]);
}

TEST(func, getIsSink) {
Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                        {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
   /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
   /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* v     |           vv v  */
   /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
   /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
   /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
   /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> sink = getSink(graph);
  sort(sink.begin(), sink.end());
  EXPECT_EQ(2, sink.size());
  EXPECT_EQ(8, sink[0]);
  EXPECT_EQ(9, sink[1]);
}

TEST(func, getIsolated) {
Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                        {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
   /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
   /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
   /* v     |           vv v  */
   /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
   /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
   /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
   /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> isolated = getIsolated(graph);
  sort(isolated.begin(), isolated.end());
  EXPECT_EQ(1, isolated.size());
  EXPECT_EQ(4, isolated[0]);
}

//Currently function isn't in use
TEST(func, hasSelfLoop) {
  Graph graph0(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
                                           {1, 1, 0, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_FALSE(hasSelfLoop(graph0));
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_TRUE(hasSelfLoop(graph1));
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_TRUE(hasSelfLoop(graph2));
}

TEST(func, hasLoop) {
  Graph graph0(buildSimpleAdjacencyMatrix({{0, 0, 0},
         /* 1    0    2 */                 {0, 0, 0},
                                           {0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph0));
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 1},
         /* 1<---0--->2 */                 {0, 0, 0},
                                           {0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph1));
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 1},
         /* 1<-->0 */                      {1, 0}}));
  EXPECT_TRUE(hasLoop(graph2));
  Graph graph3(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},     // 0-->1
                                           {0, 0, 1, 0},     //   
                                           {0, 0, 0, 1},     // 2<->3
                                           {1, 0, 0, 0}}));   
  EXPECT_TRUE(hasLoop(graph3));
  Graph graph4(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},     // 0-->1
                                           {0, 0, 1, 0},     // ^   |
                                           {0, 0, 0, 1},     // |   v  
                                           {1, 0, 0, 0}}));  // 3<--2
  EXPECT_TRUE(hasLoop(graph4));
  Graph graph5(buildSimpleAdjacencyMatrix({{1}}));
  EXPECT_TRUE(hasLoop(graph5));
  Graph graph6(buildSimpleAdjacencyMatrix({{0}}));
  EXPECT_FALSE(hasLoop(graph6));
  Graph graph7akaS3t40(buildSimpleAdjacencyMatrix({{0, 0, 0},      //0
                                                   {0, 0, 1},      //
                                                   {0, 1, 0}}));   //1<-->2
  EXPECT_TRUE(hasLoop(graph7akaS3t40));
  Graph graph8akaManual(buildSimpleAdjacencyMatrix({{0, 0, 0, 0, 0, 0},
      /*  0  1--->3           */                    {0, 0, 0, 1, 0, 0},
      /*          ^           */                    {0, 0, 0, 1, 1, 0},
      /*          |           */                    {0, 0, 0, 0, 0, 0},
      /*          2--->4--->5 */                    {0, 0, 0, 0, 0, 1},
                                                    {0, 0, 0, 0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph8akaManual));
}

TEST(func, manual_getCompletionToStrongСonnectivityInСondensed) {
  vector<vector<bool>> matrix = {{0, 0, 0, 0, 0, 0},
  /*  0  1--->3           */     {0, 0, 0, 1, 0, 0},
  /*          ^           */     {0, 0, 0, 1, 1, 0},
  /*          |           */     {0, 0, 0, 0, 0, 0},
  /*          2--->4--->5 */     {0, 0, 0, 0, 0, 1},
                                 {0, 0, 0, 0, 0, 0}};
  Graph graph(buildSimpleAdjacencyMatrix(matrix));
  vector<pair<int,int>> completion;
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(3, completion.size());
  for (auto it = completion.begin(); it != completion.end(); ++it)
    matrix[it->first][it->second] = 1;
  Graph completed_graph(buildSimpleAdjacencyMatrix(matrix));
  Coloring components = getStronglyConnectedComponentsDummy(completed_graph);
  EXPECT_EQ(1, components.getNumberOfColors());
}

TEST(func, death_getCompletionToStrongСonnectivityInСondensed) {
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
        /* has loops */                    {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_DEATH({getCompletionToStrongСonnectivityInСondensed(graph1);}, "");
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 0, 0, 0},
                                           {0, 0, 0, 1},
      /*  0      1--->3<---2     */        {0, 0, 0, 1},
      /* more sources than sinks */        {0, 0, 0, 0}}));
  EXPECT_DEATH({getCompletionToStrongСonnectivityInСondensed(graph2);}, "");
}

TEST(func, s1_getCompletionToStrongСonnectivityInСondensed) {
  Graph graph(buildSimpleAdjacencyMatrix({{0}}));
  vector<pair<int,int>> completion;
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(0, completion.size());
}

TEST(func, s2_getCompletionToStrongСonnectivityInСondensed) {
  vector<pair<int,int>> completion;
  pair<int,int> e01(0, 1);
  pair<int,int> e10(1, 0);
  Graph graph(buildSimpleAdjacencyMatrix({{0, 0},
                                          {0, 0}})); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  sort(completion.begin(), completion.end());
  EXPECT_EQ(2, completion.size()); 
  EXPECT_EQ(e01, completion[0]);
  EXPECT_EQ(e10, completion[1]);

  graph = buildSimpleAdjacencyMatrix({{0, 1},
                                      {0, 0}}); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(1, completion.size()); 
  EXPECT_EQ(e10, completion[0]);

  graph = buildSimpleAdjacencyMatrix({{0, 0},
                                      {1, 0}}); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(1, completion.size()); 
  EXPECT_EQ(e01, completion[0]);
}

/** genMatrixByBoolMask(tuple<int,int> mask) Generates matrix by given mask
*
* get<0>(mask) -> size of graph
* get<1>(mask) -> bitmask, that describes graph
*
* for example, mask = 749, size = 4
*
* 749 = 1011101101
*      #9876543210
* we enumerate bits from the lowest
*
*   bits correspond to the   |   so adjacency matrix for (4; 749)
* adjacency matrix like this |        will look like this
*    12  0  1  2             |              0 1 0 1
*     6 13  3  4             |              1 0 1 0
*     7  8 14  5             |              1 0 0 1
*     9 10 11 15             |              1 0 0 0
*
* So, masks from [0; 2 ^ (size * (size - 1) / 2) ) = topsorted masks
* all upper triangular adjacency matrixes with zeros on the main diagonal
*
* masks from [0; 2 ^ (size * (size - 1)) ) = masks without self-loops
* all adjacency matrixes with zeros on the main diagonal
*/
vector<vector<bool>> genMatrixByBoolMask(tuple<int,int> mask) {
  int size = std::get<0>(mask);
  int bitmask = std::get<1>(mask);
  vector<vector<bool>> matrix(size, vector<bool>(size, 0));
  int currentbit = 0;
  for(int i = 0; i < size; ++i)
    for (int j = i + 1; j < size; ++j) {
      matrix[i][j] = (bitmask & (1 << currentbit)) > 0;
      ++currentbit;
    }
  for(int i = 0; i < size; ++i)
    for (int j = 0; j < i; ++j) {
      matrix[i][j] = (bitmask & (1 << currentbit)) > 0;
      ++currentbit;
    }
  for (int i = 0; i < size; ++i) {
     matrix[i][i] = (bitmask & (1 << currentbit)) > 0;
     ++currentbit;
  }
  return matrix;
}

TEST(helpers, genMatrixByBoolMask) {
  tuple<int,int> mask_from_comments_to_source(4, 749);
  vector<vector<bool>> expected_matix = {{0, 1, 0, 1},
                                         {1, 0, 1, 0},
                                         {1, 0, 0, 1},
                                         {1, 0, 0, 0}};
  EXPECT_EQ(expected_matix, genMatrixByBoolMask(mask_from_comments_to_source));
}

namespace std {
std::ostream& operator << (std::ostream& os, const tuple<int,int>& val) {
  os << std::get<0>(val) << " " << std::get<1>(val);
  return os;
}
}//namespace std

void runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int> mask) {
  vector<vector<bool>> matrix = genMatrixByBoolMask(mask);
  Graph graph = buildSimpleAdjacencyMatrix(matrix);
  vector<int> source = getSource(graph);
  vector<int> sink = getSink(graph);
  vector<int> isolated = getIsolated(graph);
  if ((!hasLoop(graph)) && (source.size() <= sink.size())) {
    vector<pair<int,int>> completion;
    completion = getCompletionToStrongСonnectivityInСondensed(graph);
    for (auto it = completion.begin(); it != completion.end(); ++it) {
      matrix[(*it).first][(*it).second] = 1;
    }
    Graph completed = buildSimpleAdjacencyMatrix(matrix);
    Coloring components = getStronglyConnectedComponentsDummy(completed);
    ASSERT_EQ(isolated.size() + sink.size(), completion.size()) << mask;
    ASSERT_EQ(1, components.getNumberOfColors()) << mask;
  }
}

typedef ::testing::TestWithParam<int> StressNoSelfLoops;
TEST_P(StressNoSelfLoops, getCompletionToStrongСonnectivityInСondensed) {
  int size = GetParam();
  int maxmask = 1 << (size * (size - 1));
  for (int i = 0; i < maxmask; ++i)
    runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int>(size, i));
}
INSTANTIATE_TEST_CASE_P(func, StressNoSelfLoops, Range(3, 6));

typedef ::testing::TestWithParam<int> StressTopsorted;
TEST_P(StressTopsorted, getCompletionToStrongСonnectivityInСondensed) {
  int size = GetParam();
  int maxmask = 1 << (size * (size - 1) / 2);
  for (int i = 0; i < maxmask; ++i)
    runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int>(size, i));
}
INSTANTIATE_TEST_CASE_P(func, StressTopsorted, Range(6, 8));