#include <memory>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BinaryLiftingLCA.hpp"
#include "RMQLCA.hpp"
#include "OptimizedRMQLCA.hpp"
#include "TarjanLCA.hpp"

namespace py = pybind11;

std::shared_ptr<LCA> createLCA(const std::string& method) {
  if (method == "binary_lifting") return std::make_shared<BinaryLiftingLCA>();
  else if (method == "tarjan") return std::make_shared<TarjanLCA>();
  else if (method == "rmq") return std::make_shared<RMQLCA>();
  else if (method == "optimized_rmq") return std::make_shared<OptimizedRMQLCA>();
  else throw std::invalid_argument("Unknown LCA method: " + method);
}

PYBIND11_MODULE(_PyLCA, m) {
  m.doc() = "Python bindings for multiple LCA algorithms";

  py::class_<LCA, std::shared_ptr<LCA>>(m, "LCA")
    .def("init", &LCA::init, py::arg("n"))
    .def("add_edge", &LCA::add_edge, py::arg("u"), py::arg("v"))
    .def("build", &LCA::build)
    .def("getLCA", &LCA::getLCA, py::arg("u"), py::arg("v"));

  // Expose each concrete subclass
  py::class_<BinaryLiftingLCA, LCA, std::shared_ptr<BinaryLiftingLCA>>(m, "BinaryLifting")
    .def(py::init<>());

  py::class_<TarjanLCA, LCA, std::shared_ptr<TarjanLCA>>(m, "Tarjan")
    .def(py::init<>());

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