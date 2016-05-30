import hashlib
import time

stack_memos = {}
total_memos = {}
total_memos[''] = ''
trailing_memo_1 = ''
trailing_memo_2 = ''

_total = []
_current = ''
_length = 0

bricks = ['2', '3']
brick_index = 0

# Brick row operations

def add(brick):
    global _current, _length
    _current += brick
    _length += int(brick)

def delete():
    global _current, _length
    try:
        _length -= int(_current[-1])
    except IndexError:
        _length = 0
    _current = _current[:-1]
    trail()

def trail():
    global trailing_memo_2, trailing_memo_1, total_memos, _current, _total, brick_index
    #stack_memos.pop(trailing_memo_2, None)
    #del total_memos[trailing_memo_2]
    total_memos.pop(trailing_memo_2, None)
    trailing_memo_2 = trailing_memo_1
    trailing_memo_1 = str(_total) + str(brick_index)  + _current

def push():
    global _total, _current, _length
    _total.insert(0, _current)
    _current = ''
    _length = 0

def pop():
    global _total, _current, _length
    try:
        _current = _total.pop(0)
        _length = width
    except IndexError:
        _current = ''
        _length = 0


#def current(index):
#    global _current
#    if len(current) <= index:
#        return int(_current[index])
#    else:
#        return 0
#
#def prev(index):
#    global _prev
#    return int(_prev[index])

def under():
    global _length, width

    return _length < width

def exact():
    global _length, width

    return _length == width

def over():
    global _length, width

    return _length > width

def unique():
    global brick_index, _total, _current
    #s = str(brick_index) +'|' +  ''.join(_total) + '|' + _current

    h = hashlib.md5()
    s = str(_total) + str(brick_index)  + _current
    h.update(s)
    memo = h.digest()
    if memo in total_memos:
        return False
    else:
        total_memos[memo] = True
        return True

def stackable():
    global stack_memos, _current, _total

    if len(_total) == 0:
        return True

    _prev = _total[0]
    memo = _current  + _prev

    try:
        return stack_memos[memo]
    except KeyError:
        breaks = {}
        counter = 0
        for x in list(_prev):
            counter += int(x)
            breaks[counter] = True

        counter = 0
        for x in list(_current):
            counter += int(x)
            if counter in breaks and counter != width:
                stack_memos[memo] = False
                return False

    stack_memos[memo] = True
    print 'found new stack memo - ' + memo
    return True

def run():
    global bricks, brick_index, _length, height, width
    current_brick = ''
    rows = 0

    solutions = []
    
    while not (_current == '' and brick_index == len(bricks)):
        #print "current         = '" + _current + "'"
        #print "total           = " + str(_total)
        #print "brick index     = " + str(brick_index)
        #print "_length         = " + str(_length)
        #print "_rows           = " + str(rows)
        if brick_index >= len(bricks):
            brick_index = 0
            delete()
            if _length == 0:
                pop()
                rows -= 1
                if (rows < 0):
                    rows = 0
            #print 'brick index too high'
            #print '-----------------------'
            continue

        if not unique():
            #print 'not unique'
            brick_index += 1
            #print '-----------------------'
            continue

        current_brick = bricks[brick_index]
        add(current_brick)
        #print 'adding ' + current_brick

        if over():
            #print 'over, deleting'
            #print '-----------------------'
            delete()
            brick_index += 1
            continue
        elif not stackable():
            #print 'not stackable'
            delete()
            brick_index += 1
        elif exact():
            #print 'exact'
            push()
            rows += 1
            #print 'pushing new row'
            #print 'total        = ' + str(_total)
            #print 'rows          = ' + str(rows)
            if rows == height:
                #print 'found new solution = ' + str(_total)
                solutions.append(_total[:])
                pop()
                rows -= 1
                delete()
            brick_index = 0

        else:
            #print 'not exact, reseting brick index'
            brick_index = 0


        #print '-----------------------'
    return len(solutions)


def reset():
    global stack_memos, total_memos, _total, _current, _length, brick_index
    global trailing_memo_1, trailing_memo_2
    stack_memos.clear()
    total_memos.clear()
    _total = []
    _current = ''
    _length = 0
    brick_index = 0
    trailing_memo_1 = ''
    trailing_memo_2 = ''

if __name__ == "__main__":
    global height, width
    for i in range(1, 11):
        height = i
        width = 32
        time1 = time.time()
        solutions = run()

        time2 = time.time()
        print 'height      = ' + str(height)
        print 'solutions   = ' + str(solutions)
        print 'time delta  = %0.3f seconds' % (time2 - time1)

        reset()
