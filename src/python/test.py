import unittest
import timeit
import os

import _PyLCA
from _PyLCA import LCAOptions, QueryMode, Preference

class Test(unittest.TestCase):
    def create_LCA(self, opts):
        return _PyLCA.createLCA(opts)
    
    def generate_BinaryLiftingLCA_options(self):
        opts = LCAOptions()
        opts.mode = QueryMode.ONLINE
        opts.pref = Preference.MEMORY
        return opts
    
    def generate_TarjanLCA_options(self):
        opts = LCAOptions()
        opts.mode = QueryMode.OFFLINE
        opts.pref = Preference.QUERY_TIME
        return opts
    
    def generate_RMQLCA_options(self):
        opts = LCAOptions()
        opts.n = 40000
        opts.mode = QueryMode.ONLINE
        opts.pref = Preference.QUERY_TIME
        return opts
    
    def generate_OptimizedRMQLCA_options(self):
        opts = LCAOptions()
        opts.n = 60000
        opts.mode = QueryMode.ONLINE
        opts.pref = Preference.QUERY_TIME
        return opts
    
    def parse_testcase(self, testcase):
        if not os.path.exists(testcase):
            raise FileNotFoundError(f"File {testcase} does not exist.")
        # return a tuple (nodes, query, edges, queries)
        with open(testcase, 'r') as f:
            lines = f.readlines()
        nodes, query = map(int, lines[0].strip().split())
        edges = []
        for line in lines[1:nodes]:
            u, v = map(int, line.strip().split())
            edges.append((u, v))
        queries = []
        for line in lines[nodes:]:
            u, v = map(int, line.strip().split())
            queries.append((u, v))
        return nodes, query, edges, queries

    def check_correctness(self, LCA, test_num, tarjan=False):

        testcase = '../../testcases/' + str(test_num) + '.txt'
        nodes, query, edges, queries = self.parse_testcase(testcase)

        answer = '../../testcases/answer/' + str(test_num) + '.out'
        with open(answer, 'r') as f:
            expected = [int(line.strip()) for line in f.readlines()]

        LCA.init(nodes)
        for u, v in edges:
            LCA.add_edge(u, v)

        if tarjan:
            for i in range(len(queries)):
                LCA.add_query(queries[i][0], queries[i][1], i)

        LCA.preprocess()

        result = []
        for u, v in queries:
            result.append(LCA.getLCA(u, v))

        self.assertEqual(result, expected)

    def test_small_tree(self):

        opts = self.generate_BinaryLiftingLCA_options()
        BinaryLiftingLCA = self.create_LCA(opts)

        opts = self.generate_TarjanLCA_options()
        TarjanLCA = self.create_LCA(opts)

        opts = self.generate_RMQLCA_options()
        RMQLCA = self.create_LCA(opts)

        opts = self.generate_OptimizedRMQLCA_options()
        OptimizedRMQLCA = self.create_LCA(opts)

        self.check_correctness(BinaryLiftingLCA, 1)
        self.check_correctness(TarjanLCA, 1, tarjan=True)
        self.check_correctness(RMQLCA, 1)
        self.check_correctness(OptimizedRMQLCA, 1)

    def test_large_tree(self):

        for test_num in [2, 3, 4, 6, 7]:

            opts = self.generate_BinaryLiftingLCA_options()
            BinaryLiftingLCA = self.create_LCA(opts)

            opts = self.generate_TarjanLCA_options()
            TarjanLCA = self.create_LCA(opts)

            opts = self.generate_RMQLCA_options()
            RMQLCA = self.create_LCA(opts)

            opts = self.generate_OptimizedRMQLCA_options()
            OptimizedRMQLCA = self.create_LCA(opts)

            self.check_correctness(BinaryLiftingLCA, test_num)
            self.check_correctness(TarjanLCA, test_num, tarjan=True)
            self.check_correctness(RMQLCA, test_num)
            self.check_correctness(OptimizedRMQLCA, test_num)

    def test_edge_cases(self):

        opts = self.generate_BinaryLiftingLCA_options()
        BinaryLiftingLCA = self.create_LCA(opts)

        opts = self.generate_TarjanLCA_options()
        TarjanLCA = self.create_LCA(opts)

        opts = self.generate_RMQLCA_options()
        RMQLCA = self.create_LCA(opts)

        opts = self.generate_OptimizedRMQLCA_options()
        OptimizedRMQLCA = self.create_LCA(opts)

        self.check_correctness(BinaryLiftingLCA, 5)
        self.check_correctness(TarjanLCA, 5, tarjan=True)
        self.check_correctness(RMQLCA, 5)
        self.check_correctness(OptimizedRMQLCA, 5)

    def test_performance(self):
        
        opts = self.generate_BinaryLiftingLCA_options()
        BinaryLiftingLCA = self.create_LCA(opts)

        opts = self.generate_TarjanLCA_options()
        TarjanLCA = self.create_LCA(opts)

        opts = self.generate_RMQLCA_options()
        RMQLCA = self.create_LCA(opts)

        opts = self.generate_OptimizedRMQLCA_options()
        OptimizedRMQLCA = self.create_LCA(opts)




# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4: