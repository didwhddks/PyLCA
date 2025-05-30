import _PyLCA

def test():
    # Test the _PyLCA module
    print("Testing _PyLCA module...")
    
    # Example usage of a function from _PyLCA
    LCA = _PyLCA.createLCA("optimized_rmq")
    LCA.init(5)
    LCA.add_edge(1, 2)
    LCA.add_edge(1, 3)
    LCA.add_edge(2, 4)
    LCA.add_edge(2, 5)

    LCA.build()
    print(LCA.getLCA(3, 5))
    print(LCA.getLCA(4, 5))
    print("LCA created and edges added successfully.")

if __name__ == "__main__":
    test()
    print("Test completed successfully.")