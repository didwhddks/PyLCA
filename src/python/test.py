import unittest
import timeit
import os

import _PyLCA
from _PyLCA import LCAOptions, QueryMode, Preference

import matplotlib.pyplot as plt

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
    
    def calc_qps(self, LCA, testcase, tarjan=False):
        nodes, query, edges, queries = self.parse_testcase(testcase)

        LCA.init(nodes)
        for u, v in edges:
            LCA.add_edge(u, v)

        if tarjan:
            for i in range(len(queries)):
                LCA.add_query(queries[i][0], queries[i][1], i)

        start_time = timeit.default_timer()
        LCA.preprocess()
        preprocess_time = timeit.default_timer() - start_time

        start_time = timeit.default_timer()
        for u, v in queries:
            LCA.getLCA(u, v)
        query_time = timeit.default_timer() - start_time

        qps = len(queries) / query_time
        return qps, preprocess_time * 1000

    def test_performance(self):

        # calculate QPS and memory usage for each LCA implementation
        BinaryLiftingLCA_avg_qps = 0
        BinaryLiftingLCA_avg_preprocess_time = 0
        for test_num in [2, 3, 4, 6, 7]:
            testcase = '../../testcases/' + str(test_num) + '.txt'
            nodes, query, edges, queries = self.parse_testcase(testcase)

            opts = self.generate_BinaryLiftingLCA_options()
            BinaryLiftingLCA = self.create_LCA(opts)

            qps, preprocess_time = self.calc_qps(BinaryLiftingLCA, testcase)

            BinaryLiftingLCA_avg_qps += qps
            BinaryLiftingLCA_avg_preprocess_time += preprocess_time
        
        BinaryLiftingLCA_avg_qps /= 5
        BinaryLiftingLCA_avg_preprocess_time /= 5

        TarjanLCA_avg_qps = 0
        TarjanLCA_avg_preprocess_time = 0
        for test_num in [2, 3, 4, 6, 7]:
            testcase = '../../testcases/' + str(test_num) + '.txt'
            nodes, query, edges, queries = self.parse_testcase(testcase)

            opts = self.generate_TarjanLCA_options()
            TarjanLCA = self.create_LCA(opts)

            qps, preprocess_time = self.calc_qps(TarjanLCA, testcase, tarjan=True)

            TarjanLCA_avg_qps += qps
            TarjanLCA_avg_preprocess_time += preprocess_time

        TarjanLCA_avg_qps /= 5
        TarjanLCA_avg_preprocess_time /= 5

        RMQLCA_avg_qps = 0
        RMQLCA_avg_preprocess_time = 0
        for test_num in [2, 3, 4, 6, 7]:
            testcase = '../../testcases/' + str(test_num) + '.txt'
            nodes, query, edges, queries = self.parse_testcase(testcase)

            opts = self.generate_RMQLCA_options()
            RMQLCA = self.create_LCA(opts)

            qps, preprocess_time = self.calc_qps(RMQLCA, testcase)

            RMQLCA_avg_qps += qps
            RMQLCA_avg_preprocess_time += preprocess_time
        
        RMQLCA_avg_qps /= 5
        RMQLCA_avg_preprocess_time /= 5

        OptimizedRMQLCA_avg_qps = 0
        OptimizedRMQLCA_avg_preprocess_time = 0
        for test_num in [2, 3, 4, 6, 7]:
            testcase = '../../testcases/' + str(test_num) + '.txt'
            nodes, query, edges, queries = self.parse_testcase(testcase)

            opts = self.generate_OptimizedRMQLCA_options()
            OptimizedRMQLCA = self.create_LCA(opts)

            qps, preprocess_time = self.calc_qps(OptimizedRMQLCA, testcase)

            OptimizedRMQLCA_avg_qps += qps
            OptimizedRMQLCA_avg_preprocess_time += preprocess_time

        OptimizedRMQLCA_avg_qps /= 5
        OptimizedRMQLCA_avg_preprocess_time /= 5

        # plot the QPS results
        labels = ['Binary Lifting', 'Tarjan', 'RMQ', 'Optimized RMQ']
        qps = [BinaryLiftingLCA_avg_qps, TarjanLCA_avg_qps, RMQLCA_avg_qps, OptimizedRMQLCA_avg_qps]

        x_positions = range(len(labels))
        plt.figure(figsize=(10, 6))

        bars = plt.bar(x_positions, qps, edgecolor='black', width=0.4)

        for bar in bars:
            height = bar.get_height()
            plt.text(
                bar.get_x() + bar.get_width() / 2,
                height + max(qps) * 0.01,
                f'{height:.1f}',
                ha='center', va='bottom', fontsize=10
            )

        plt.xlabel('LCA Implementations', fontsize=12)
        plt.ylabel('Queries Per Second (QPS)', fontsize=12)
        plt.title('Performance Comparison', fontsize=14)

        plt.xticks(x_positions, labels, rotation=0, fontsize=10)
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        plt.tight_layout()
        plt.savefig('../../results/performance_comparison.png', dpi=300)
        plt.show()

        # plot the preprocess time results
        preprocess_times = [
            BinaryLiftingLCA_avg_preprocess_time,
            TarjanLCA_avg_preprocess_time,
            RMQLCA_avg_preprocess_time,
            OptimizedRMQLCA_avg_preprocess_time
        ]
        plt.figure(figsize=(10, 6))
        bars = plt.bar(x_positions, preprocess_times, edgecolor='black', width=0.4)
        for bar in bars:
            height = bar.get_height()
            plt.text(
                bar.get_x() + bar.get_width() / 2,
                height + max(preprocess_times) * 0.01,
                f'{height:.2f}ms',
                ha='center', va='bottom', fontsize=10
            )
        plt.xlabel('LCA Implementations', fontsize=12)
        plt.ylabel('Preprocess Time (ms)', fontsize=12)
        plt.title('Preprocess Time Comparison', fontsize=14)
        plt.xticks(x_positions, labels, rotation=0, fontsize=10)
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        plt.tight_layout()
        plt.savefig('../../results/preprocess_time_comparison.png', dpi=300)
        plt.show()

# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4: