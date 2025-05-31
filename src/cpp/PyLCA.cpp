#include <memory>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BinaryLiftingLCA.hpp"
#include "RMQLCA.hpp"
#include "OptimizedRMQLCA.hpp"
#include "TarjanLCA.hpp"
#include "LCAOptions.hpp"

namespace py = pybind11;

inline std::shared_ptr<LCA> createLCA(const LCAOptions& opts) {
  if (opts.mode == QueryMode::OFFLINE) {
    if (opts.pref == Preference::BUILD_TIME || opts.pref == Preference::QUERY_TIME) {
      return std::make_shared<TarjanLCA>();
    } else if (opts.pref == Preference::MEMORY) {
      if (opts.q > 4 * opts.n) {
        return std::make_shared<BinaryLiftingLCA>();
      } else {
        return std::make_shared<TarjanLCA>();
      }
    }
  } else if (opts.mode == QueryMode::ONLINE) {
    if (opts.pref == Preference::MEMORY) {
      return std::make_shared<BinaryLiftingLCA>();
    }
    if (opts.pref == Preference::QUERY_TIME) {
      if (opts.n > 50000) {
        return std::make_shared<OptimizedRMQLCA>();
      } else {
        return std::make_shared<RMQLCA>();
      }
    }
    if (opts.pref == Preference::BUILD_TIME) {
      return std::make_shared<BinaryLiftingLCA>();
    }
  }
  throw std::invalid_argument("Cannot pick an LCA algorithm for the given options.");
}


PYBIND11_MODULE(_PyLCA, m) {
  m.doc() = "Python bindings for multiple LCA algorithms";

  py::enum_<QueryMode>(m, "QueryMode")
    .value("ONLINE", QueryMode::ONLINE)
    .value("OFFLINE", QueryMode::OFFLINE)
    .export_values();

  py::enum_<Preference>(m, "Preference")
    .value("BUILD_TIME", Preference::BUILD_TIME)
    .value("QUERY_TIME", Preference::QUERY_TIME)
    .value("MEMORY", Preference::MEMORY)
    .export_values();

  py::class_<LCAOptions>(m, "LCAOptions")
    .def(py::init<>(), 
          R"( LCAOptions(n=0, q=0, mode=QueryMode.ONLINE, pref=Preference.QUERY_TIME)
          Create an options bundle for picking an LCA algorithm.

          :param n:    Expected number of nodes (int)
          :param q:    Expected number of queries (int)
          :param mode: QueryMode.ONLINE or QueryMode.OFFLINE
          :param pref: Preference.BUILD_TIME, Preference.QUERY_TIME, or Preference.MEMORY
          )")
    .def_readwrite("n", &LCAOptions::n, "Expected number of nodes")
    .def_readwrite("q", &LCAOptions::q, "Expected number of queries")
    .def_readwrite("mode", &LCAOptions::mode, "Query mode (ONLINE or OFFLINE)")
    .def_readwrite("pref", &LCAOptions::pref, "Preference for algorithm selection (BUILD_TIME, QUERY_TIME, MEMORY)");

  m.def("createLCA", [](const LCAOptions& opts) {
    return createLCA(opts);
  });

  py::class_<LCA, std::shared_ptr<LCA>>(m, "LCA")
    .def("init", &LCA::init, py::arg("n"))
    .def("add_edge", &LCA::add_edge, py::arg("u"), py::arg("v"))
    .def("preprocess", &LCA::preprocess)
    .def("getLCA", &LCA::getLCA, py::arg("u"), py::arg("v"));

  // Expose each concrete subclass
  py::class_<BinaryLiftingLCA, LCA, std::shared_ptr<BinaryLiftingLCA>>(m, "BinaryLifting")
    .def(py::init<>());

  py::class_<TarjanLCA, LCA, std::shared_ptr<TarjanLCA>>(m, "Tarjan")
    .def(py::init<>())
    .def("add_query", &TarjanLCA::add_query, 
         py::arg("u"), py::arg("v"), py::arg("idx"));

  py::class_<RMQLCA, LCA, std::shared_ptr<RMQLCA>>(m, "RMQ")
    .def(py::init<>());

  py::class_<OptimizedRMQLCA, LCA, std::shared_ptr<OptimizedRMQLCA>>(m, "OptimizedRMQ")
    .def(py::init<>());

  // Factory function
  m.def("createLCA", &createLCA, 
        py::arg("method"),
        "Create an LCA solver instance based on the specified method. "
        "Available methods: 'binary_lifting', 'tarjan', 'rmq', 'optimized_rmq'.");
}