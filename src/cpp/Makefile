VENV      := venv
PYTHON    := $(VENV)/bin/python
PIP       := $(VENV)/bin/pip

PYBIND_INC := $(shell $(PYTHON) -m pybind11 --includes)
NUMPY_INC  := -I$(shell $(PYTHON) -c "import numpy; print(numpy.get_include())")

CXX        := g++
CXXFLAGS   := -O3 -Wall -shared -std=c++17 -fPIC -fopenmp \
              $(PYBIND_INC) $(NUMPY_INC)

all: set_up_venv _PyLCA.so

set_up_venv:
	if [ ! -d $(VENV) ]; then \
	  python3 -m venv $(VENV) && $(PIP) install pytest pybind11 numpy; \
	fi

test: set_up_venv _PyLCA.so
	$(PYTHON) ../python/test.py

_PyLCA.so: PyLCA.o BinaryLiftingLCA.o TarjanLCA.o DSU.o SegmentTree.o RMQLCA.o OptimizedRMQLCA.o
	$(CXX) $(CXXFLAGS) $^ -o ../python/_PyLCA.so

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(VENV) *.so __pycache__ *.pyc .pytest_cache *.o