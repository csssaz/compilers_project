import os
import time

def test_basic():
  os.system('./build/DecafComp')
  time.sleep(1)
  os.system('java -cp ./JTacInt.jar is.ru.tacint.TacBlock test.tac > temp.txt')
  time.sleep(1)
  with open('temp.txt', 'r') as f:
    res = [l.strip() for l in f][2:]
    expected = ['6', '1', '0', '1212', '1111', '666', '2', '1', '3.0', '2.0']
    print('got:\t\t{}'.format(','.join(res)))
    print('expected:\t{}'.format(','.join(expected)))
    assert res == expected, 'woopxx'
  os.remove('temp.txt')

def test_all_files():
  filenames = ['test_files/t'+str(i) for i in range(1, 9)]
  expected = [
      ['6'],
      ['24'],
      ['1'],
      ['0', '1', '2', '3'],
      ['100', '0', '1', '2', '100', '1', '0', '2',
       '100', '2', '0', '1', '100', '3', '0', '1', '2'],
      ['8', '0', '0'],
      ['8.0', '0.0', '0.5'],
      ['1', '0', '0', '300', '1', '400', '1']]
  for filename, expected_output in zip(filenames, expected):
    os.system('./build/DecafComp asdf {}.decaf'.format(filename))
    time.sleep(0.2)
    os.system('java -cp ./JTacInt.jar is.ru.tacint.TacBlock {}.tac > temp.txt'.format(filename))
    time.sleep(0.5)
    with open('temp.txt', 'r') as f:
      res = [l.strip() for l in f][2:]
      print('got:\t\t{}'.format(','.join(res)))
      print('expected:\t{}'.format(','.join(expected_output)))
      assert res == expected_output, 'woopzzz'
    os.remove('temp.txt')

if __name__ == '__main__':
  test_basic()
  test_all_files()
