import os
import time

def test_basic():
  os.system('./build/DecafComp')
  time.sleep(1)
  os.system('java -cp ./JTacInt.jar is.ru.tacint.TacBlock test.tac > temp.txt')
  time.sleep(1)
  with open('temp.txt', 'r') as f:
    res = [l.strip() for l in f][2:]
    expected = ['6', '1', '0', '1212', '1111', '666', '2', '1']
    print('got:\t\t{}'.format(','.join(res)))
    print('expected:\t{}'.format(','.join(expected)))
    assert res == expected, 'woopxx'
  os.remove('temp.txt')

if __name__ == '__main__':
  test_basic()

