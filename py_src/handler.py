import hashlib

stack_memos = {}
total_memos = {}

_total = []
_current = 0
_length = 0

bricks = [2, 3]
brick_index = 0

height = 10
width = 32

# Brick row operations

def add(brick):
    global _current, _length
    _current = _current * 10
    _current += brick
    _length += brick

def delete():
    global _current, _length
    _length -= (_current % 10)
    _current //= 10

def push():
    global _total, _current, _length
    _total.insert(0, _current)
    _current = 0
    _length = 0

def pop():
    global _total, _current, _length
    try:
        _current = _total.pop(0)
        _length = width
    except IndexError:
        _current = 0
        _length = 0


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
    s = str(brick_index) + '|' + str(_total) + '|' + str(_current)
    h = hashlib.md5()
    h.update(s)
    s = h.digest()
    if s in total_memos:
        return False
    else:
        total_memos[s] = True
        return True

def stackable():
    global stack_memos, _current, _total

    if len(_total) == 0:
        return True

    _prev = _total[0]
    memo = str(_current) + '|' + str(_prev)

    try:
        return stack_memos[memo]
    except KeyError:
        cur_breaks = _length
        prev_breaks = width
        cur = _current
        #TODO FIX ME
        prev = _prev

        while cur and prev:
            #print 'cur_breaks = ' + str(cur_breaks)
            #print 'prev_breaks = ' + str(prev_breaks)
            #print 'cur = ' + str(cur)
            #print 'prev = ' + str(prev)
            if (prev_breaks < prev_breaks):
                cur_breaks -= (cur % 10)
                cur //= 10
                if cur_breaks != 0 and cur_breaks == prev_breaks:
                    stack_memos[memo] = False
                    return False
            else:
                prev_breaks -= (prev % 10)
                prev //= 10
                if prev_breaks != 0 and prev_breaks == cur_breaks:
                    stack_memos[memo] = False
                    return False

        stack_memos[memo] = True
        return True

def main():
    global bricks, brick_index, _length
    current_brick = 0
    rows = 0

    solutions = []
    
    while not (_current == 0 and brick_index == len(bricks)):
        #print "current         = " + str(_current)
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
        #print 'adding ' + str(current_brick)

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
    print 'found ' + str(len(solutions)) + ' solutions'


if __name__ == "__main__":
    main()
