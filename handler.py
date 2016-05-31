# Jake Mingolla
# May 31st, 2016
# GreatHorn Coding Test
#
# README
#
# This script calculates the number of unique solutions to build walls
# made of bricks. For the sake of avoiding rounding errors for floating
# precision numbers, all bricks must be reduced to relatively prime integer
# components. In this example, the blocks of size 3 and 4.5 may be reduced to
# size 2 and 3 blocks respectively, as both of these numbers are relatively
# prime and form the fraction 2/3. All blocks are assumed to have a height
# of 1, so the blocks are only distorted in the x-direction (width). In addition,
# the blocks are chosen to be sizes 1 - 9. Choosing a block of size 0 will
# lead to a very bad time.
#
# At each step, we keep track of the total block configuration as well as
# the current row we are adding. Bricks are stored as characters in strings
# for easy access using python helper functions such as join and array
# accessing. In addition, a brick_index as well as a list of possible
# bricks is maintained in order to decide which brick to place at each step
# of the invocation. Each step involves adding a new brick to the wall,
# seeing if the location is unique, stackable, and exactly the width of 
# the wall. If a position is not unique, it is discarded and the most
# recently placed brick is removed from the current row. Similarily, if
# the most recently placed brick yields an un-stackable solution, it is
# removed. If we have not yet reached the end of the wall, we keep building -
# if we have exceeded it, we remove the last brick. Once all bricks are
# placed in a stable configuration for every row, we have found a solution.
#
# In order to avoid repition, the script utilies two memoization caches to 
# store repeated calculates. Because of the optimal substructure of the
# problem, we can use these dynamic programming techniques to not only ignore
# recalculating positions we have already visited but also check the minimum
# number of stacking configurations. In this example using size 2 and 3 blocks
# on a width of 38 feet, there are 421722 ways to arrange two rows of blocks
# and have them fit exactly within the bounds of the walls, but only 37210
# are valid solutions. 
#
# By memoizing the stacking configurations, we provide an upper bound on the
# number of comparisons that can be made (and thus stored) when checking if
# a stacking configuration is possible during an arbitrary point of the program's
# runtime. When checking to see if two rows can stack on top of one-another,
# first the dictionary of memos is checked with a hash composed of the two
# rows. If it has already been calculated, we can simply use the prior result
# without having to calculate it. If not, we calculate whether or not two rows
# can stack on eachother by iterating through and keeping track of the breaks in
# the rows and adding the result to the dictionary.
#
# The same memoization process is used to calculate if we have visted a
# given configuration of the brick arrangement before. At every step
# of the program, we hash the current row, the rest of the wall, and what brick
# we are attempting to add to the row and add it to a list of the total
# unique positions. If we ever calculate a hash of an existing position,
# we know we have reached a non-unique configuration and we can roll
# back the most recently placed brick.
#
#
#
# USAGE
#
# python handler.py
# 
# python runtime - 2.7

import hashlib
import time

stack_memos = {}
total_memos = {}

_total = []
_current = ''
_length = 0

bricks = ['2', '3']
brick_index = 0

trailing_counter = 0
trailing_memo = ''

def create_memo():
    global _total, brick_index, _current
    return str(_total) + str(brick_index)  + _current

def create_hash(x):
    h = hashlib.md5()
    h.update(x)
    return h.digest()

def add(brick):
    global _current, _length
    global trailing_counter

    trailing_counter = 0

    _current += brick
    _length += int(brick)


def trail():
    global trailing_counter, total_memos

    trailing_counter += 1
    if (trailing_counter > 1):
        trailing_memo = create_memo()
        trailing_hash = create_hash(trailing_memo)
        del total_memos[trailing_hash]



def delete():
    global _current, _length
    trail()

    try:
        _length -= int(_current[-1])
    except IndexError:
        _length = 0
    _current = _current[:-1]


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

def exact():
    global _length, width
    return _length == width

def over():
    global _length, width
    return _length > width

def unique():
    global brick_index, _total, _current

    memo = create_hash(create_memo())
    if memo in total_memos:
        return False
    else:
        total_memos[memo] = True
        return True

def stackable():
    global stack_memos

    if len(_total) == 0:
        return True

    prev = _total[0]
    memo = create_hash(_current + prev)

    if memo in stack_memos:
        return stack_memos[memo]
    else:
        return breaks_exist(memo)

def breaks_exist(memo):
    global stack_memos, _total, _current
    prev = _total[0]
    breaks = {}
    counter = 0

    for x in list(prev):
        counter += int(x)
        breaks[counter] = True

    counter = 0

    for x in list(_current):
        counter += int(x)
        if counter in breaks and counter != width:
            stack_memos[memo] = False
            return False

    stack_memos[memo] = True
    return True

def run():
    global bricks, brick_index, _length, height, width
    current_brick = ''
    rows = 0

    solutions = 0
    
    while not (_current == '' and brick_index == len(bricks)):
        if brick_index >= len(bricks):
            brick_index = 0
            delete()
            if _length == 0:
                pop()
                rows -= 1
                if (rows < 0):
                    rows = 0
            continue

        if not unique():
            brick_index += 1
            continue

        current_brick = bricks[brick_index]
        add(current_brick)

        if over() or not stackable():
            delete()
            brick_index += 1
            continue
        elif exact():
            push()
            rows += 1
            if rows == height:
                solutions += 1
                pop()
                rows -= 1
                delete()
            brick_index = 0

        else:
            brick_index = 0


    return solutions


def init(width, height):
    global stack_memos, total_memos, _total, _current, _length, brick_index
    
    # Technically, since the stack_memos are the same for every configuration
    # with the same width, there is no reason to clear it here. I decided
    # to clear it in case the width gets changed inbetween calls to the 
    # init() function.
    stack_memos.clear()
    total_memos.clear()
    _total = []
    _current = ''
    _length = 0
    brick_index = 0
    counter = 0

if __name__ == "__main__":
    global height, width
    for i in range(1, 11):

        height = i
        width = 32

        init(width, height)

        time1 = time.time()

        solutions = run()

        time2 = time.time()
        print 'height      = ' + str(height)
        print 'solutions   = ' + str(solutions)
        print len(stack_memos.keys())
        print 'time delta  = %0.3f seconds' % (time2 - time1)

