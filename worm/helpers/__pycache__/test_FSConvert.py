import unittest
import FSConvert

class TestFSConvert(unittest.TestCase):
    def test_ReplaceAll(self):
        filelist = ['billy', 'sally', 'joe']
        ans = ['biL1y', 'saL1y', 'joe']
        old = 'll'
        new = 'L1'
        result = FSConvert.ReplaceAll(filelist, old, new)
        self.assertEqual(result, ans)




if __name__ == '__main__':
    unittest.main()
