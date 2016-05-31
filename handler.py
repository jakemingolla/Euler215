#                              Jake Mingolla
#                             May 31st, 2016
#                           GreatHorn Coding Test
#
#                               README
#
#
#                               Description
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
# Moreover, we attempt to free memory by deleting 'trailing' configurations
# that are unreachable by the current state. Each time we delete a brick,
# we also delete the most recent configuration of the bricks calculated.
# Since we are deleting a brick, we know that all further possible solutions
# with the current configuration are impossible. Thus, we delete the trailing
# configuration to allow us to keep a more balanced memory overhead.
#
# The program continues to run until all configurations have been calculated
# for the given width and height.
#
#                               RESULTS
#
# Note: As specified above, the sizes provided in the instruction sheet have
# been reduced to integer form. So, instead of 3 and 4.5 feet long bricks
# composing a 48 foot long wall, we instead use 2 and 3 feet long bricks to
# compose a 32 foot long wall. The height is unchanged.
#
# HEIGHT            SOLUTIONS           COMPUTATION TIME (seconds)
# ------            ---------           ----------------
# 1                 3329                0.092
#
# 2                 37210               5.382
#
# 3                 592050              46.437
#
# >4                n/a                 n/a
#
# As these results show, for greater than 3 rows we run into memory leaks 
# and the program halts after running for a considerable amount of time.
# Because of the exponential blowup in the number of solutions as a function
# of the rows, I was curious to see if I could extrapolate this to estimate
# an answer for 10 rows. Using Wolfram-Alpha's best fit line given the
# above points, I have the following function to estimate the number of solutions
# given a height:
#
#       f ( x ) = solutions ( height ) = 147.387 e ^ ( 2.766 * height )
#
# We can plug in the height of 10 to estimate a total of ~ 1.518 * 10 ^ 14,
# or just over 100 trillion solutions. To even store these valid configurations 
# (and also keep in mind this estimate is only the VALID solutions, not all
# the configurations created during computation) in memory would require 
# extreme storage capacity. I am interested to see if optimizations to
# the removal of trailing impossible configurations could lead to be calculated.
#
#                               USAGE
#
# python handler.py
#
# Type each brick size as given, then press 0 to stop. Enter the width and height.
# All input must be integers. I didn't bother that much to clean input, so crazy
# stuff will almost invariably happen if a user tries to mess with the program.
# 
# python runtime - 2.7

# Imported for md5 hashing of configurations.
import hashlib

# Used for rough time calculations of runtime.
import time

# Memoization dictionaries to hold the hashed configurations for checking stackability
# and uniqueness, respectively.
stack_memos = {}
total_memos = {}

# Holds the current completed brick portion.
total = []

# Holds the leading row on which bricks can be added.
current = ''

# Holds the length (in feet) of the current row.
length = 0

# The current brick index is necessary to figure out
# what brick is going to be added next from the list of
# valid bricks.
brick_index = 0

# Holds on to the number of deletes since the last 
# add to know if we can delete current configurations.
trailing_counter = 0
trailing_memo = ''

def create_memo():
    ''' Creates a memo of the current configuration. A memo consists
        the total completed brick wall, the brick index, and the current
        row.
    ''' 
    global total, brick_index, current
    return str(total) + str(brick_index)  + current

def create_hash(x):
    ''' Creates a hash of a given input using an md5 hash.
    '''
    h = hashlib.md5()
    h.update(x)
    return h.digest()

def add(brick):
    ''' Adds a given brick to the current row and increments the
        length accordingly. Also resets the trailing counter since
        we can no longer delete past (and maybe valid) configurations.
    '''
    global current, length
    global trailing_counter

    trailing_counter = 0

    current += brick
    length += int(brick)


def trail():
    ''' If we are trailing a previously invalid solution by finding
        two deletes in a row, start deleting the total_memos of the
        invalid solutions since they will no longer be reachable and
        are taking up memory.
    '''
    global trailing_counter, total_memos

    trailing_counter += 1
    if (trailing_counter > 1):
        trailing_memo = create_memo()
        trailing_hash = create_hash(trailing_memo)
        del total_memos[trailing_hash]



def delete():
    ''' Deletes the leading brick from the current row.
    '''
    global current, length
    trail()

    try:
        length -= int(current[-1])
    except IndexError:
        length = 0
    current = current[:-1]


def push():
    ''' Pushes the current row onto the total wall configuration.
        Resets the current row string and its length.
    '''
    global total, current, length
    total.insert(0, current)
    current = ''
    length = 0

def pop():
    ''' Pops off the most recent row from the total wall and updates
        current string and length.
    '''
    global total, current, length
    try:
        current = total.pop(0)
        length = width
    except IndexError:
        current = ''
        length = 0

def exact():
    ''' Returns whether or not the current row fills the width of the wall
        exactly.
    '''
    global length, width
    return length == width

def over():
    ''' Returns whether or not the current row is over the wall limit.
    '''
    global length, width
    return length > width

def unique():
    ''' Using memoization, returns whether or not a current configuration
        has been reached using a hash of the current information.
    '''
    global brick_index, total, current

    memo = create_hash(create_memo())
    if memo in total_memos:
        return False
    else:
        total_memos[memo] = True
        return True

def stackable():
    ''' Using memoization, returns whether or not the current row can be
        stacked on the previous row (the most recently added row to the 
        total configuration. If it has previously been computed if the two
        rows are compatible, it will look in the stack_memos dictionary.
        Otherwise, the breaks in the two rows are compared and if they
        share any similarities other than the endpoints they are incompatible.
    '''
    global stack_memos

    # If there are no rows to stack on, it is by definition stackable.
    if len(total) == 0:
        return True

    prev = total[0]
    memo = create_hash(current + prev)

    if memo in stack_memos:
        return stack_memos[memo]
    else:
        return breaks_exist(memo)

def breaks_exist(memo):
    ''' Calculates if breaks exist between the current and previous rows.
        Iterates between each row and creates a break dictionary - if either
        share a break other than the width, they are incompatible. Stores
        the result under the given memo into the stack_memos dictionary
        and returns the result.
    '''
    global stack_memos, total, current
    prev = total[0]
    breaks = {}
    counter = 0

    for x in list(prev):
        counter += int(x)
        breaks[counter] = True

    counter = 0

    for x in list(current):
        counter += int(x)
        if counter in breaks and counter != width:
            stack_memos[memo] = False
            return False

    stack_memos[memo] = True
    return True

def run():
    ''' Handles logic for running the simulation. Returns the total number of
        solutions found.

        Raises a MemoryError if exceeding the memory bounds of the machine.
    '''
    global bricks, brick_index, length, height, width
    
    # The brick about to be added. Related to the possible brick dictionary.
    current_brick = ''
    
    # Number of rows we have calculated. Relates the height.
    rows = 0
    
    # Number of valid solutions
    solutions = 0
    
    # The loop ends when we have an empty current row and the brick
    # index indicates we have started all possible configurations of
    # the bricks.
    while not (current == '' and brick_index == len(bricks)):
        # If the brick index is outside the number of total bricks,
        # delete the last brick and pop if we deleted the last
        # brick of the current row.
        if brick_index >= len(bricks):
            brick_index = 0
            delete()
            if length == 0:
                pop()
                rows -= 1
                if (rows < 0):
                    rows = 0
            continue
    
        # If we have reached a non-unique solution that has been previously
        # reached, instead of adding the current brick we simply increment
        # the brick index to try all other possible bricks for the current
        # configuration of the current row and total wall.
        if not unique():
            brick_index += 1
            continue
    
        # Adds the given brick to the current row since we know we have
        # not yet seen this configuration.
        current_brick = bricks[brick_index]
        add(current_brick)
    
        # If we are over the limit or the wall or the current row
        # cannot be stacked on the previous row now that we have added
        # another brick, remove the brick and increment the brick
        # index.
        if over() or not stackable():
            delete()
            brick_index += 1
            continue
    
        # Or, if we have filled the current row, add this row
        # to the total wall since it must be stackable. If we have reached
        # the desired height of the wall, we have found a solution and the
        # current row can be popped off and deleted.
        elif exact():
            push()
            rows += 1
            if rows == height:
                solutions += 1
                pop()
                rows -= 1
                delete()
            brick_index = 0
    
        # Otherwise, we have reached a configuration that is stackable, unique,
        # and within the bounds of the row. Simply reset the brick index so
        # we know we can cover all possible configurations that branch off of
        # this one and continue the loop.
        else:
            brick_index = 0
    
    
    # Finally, return the number of solutions.
    return solutions


def init(width, height):
    global stack_memos, total_memos, total, current, length, brick_index
    
    # Technically, since the stack_memos are the same for every configuration
    # with the same width, there is no reason to clear it here. I decided
    # to clear it in case the width gets changed inbetween calls to the 
    # init() function.
    stack_memos.clear()
    total_memos.clear()
    total = []
    current = ''
    length = 0
    brick_index = 0
    counter = 0

if __name__ == "__main__":
    global height, width, bricks

    bricks = []

    # Get all valid bricks and add them to the list. Ends when a
    # brick of size 0 is inputted.
    x = -1
    while (x != 0):
        x = input('Enter a valid brick size. Press 0 to stop : ')
        if x != 0:
            bricks.append(str(x))

    height = input('Enter the height : ')
    width = input('Enter the width : ')

    # Basic sanity check
    assert(height > 0)
    assert(width > 0)

    init(width, height)

    time1 = time.time()

    solutions = run()

    time2 = time.time()
    print 'height      = ' + str(height)
    print 'solutions   = ' + str(solutions)
    print 'time delta  = %0.3f seconds' % (time2 - time1)

