#!@PYTHON@

from testenv import starrynite as sn
import random
import unittest

class TestSequenceFunctions(unittest.TestCase):
    def setUp(self):
        pass
    
    def testfandg(self):
        sn.f(5)
        self.assertEqual(sn.g(), 5)
        sn.f(10)
        self.assertEqual(sn.g(), 10)

if __name__ == '__main__':
    unittest.main()
    