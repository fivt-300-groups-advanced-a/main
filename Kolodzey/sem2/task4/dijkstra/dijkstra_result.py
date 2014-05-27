class DijkstraResult(dict):
    """
    class to hold and print result of dijkstra function
    """
    from collections import namedtuple
    FullHold = namedtuple("FullTuple", "distance parent edge")
    OnlyParentHold = namedtuple("OnlyParentTuple", "distance parent")

    #TODO add methods to get/print path
    def __init__(self, start, backtrace_mode):
        self.start = start
        self.backtrace_mode = backtrace_mode

    def add_result(self, vertex, result, edge_info):
        if self.backtrace_mode == "full":
            self[vertex] = self.FullHold(result, edge_info.parent, edge_info.edge)
        elif self.backtrace_mode == "only_parent":
            self[vertex] = self.OnlyParentHold(result, edge_info.parent)
        else:
            self[vertex] = result