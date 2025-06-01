VENV      := venv
PYTHON    := $(VENV)/bin/python
PIP       := $(VENV)/bin/pip

CXX        := g++
CXXFLAGS   := -O3 -Wall -shared -std=c++17 -fPIC -fopenmp

all: set_up_venv _PyLCA.so

set_up_venv:
	if [ ! -d $(VENV) ]; then \
	  python3 -m venv $(VENV) && $(PIP) install pytest pybind11 numpy matplotlib; \
	fi

test: set_up_venv _PyLCA.so
	$(PYTHON) -m pytest ../python/test.py -s

_PyLCA.so: PyLCA.o BinaryLiftingLCA.o TarjanLCA.o DSU.o SegmentTree.o RMQLCA.o OptimizedRMQLCA.o
	$(CXX) $(CXXFLAGS) $^ -o ../python/_PyLCA.so $(shell $(PYTHON) -m pybind11 --includes) -I$(shell $(PYTHON) -c "import numpy; print(numpy.get_include())")

PyLCA.o: PyLCA.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(shell $(PYTHON) -m pybind11 --includes) -I$(shell $(PYTHON) -c "import numpy; print(numpy.get_include())")

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(VENV) *.so __pycache__ *.pyc .pytest_cache *.o