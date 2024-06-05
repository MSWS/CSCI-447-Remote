import sys
import abc
import math
import random
from typing import List

DISK_SIZE = 10000

def main():
    global DISK_SIZE
    args = sys.argv

    if len(args) == 1:
        print("Expected Usage: main.py [R]")
        print("Expected Usage: main.py [int] [H/L] R[int]")
        print("Expected Usage: main.py [int] [H/L] [...int]")
        return

    headLocation = -1
    headDirection = False
    destinations = []

    if args[1] == 'R':
        headLocation = random.randint(0, DISK_SIZE - 1)
        headDirection = True if random.randint(0, 1) else False
        destinations = [random.randint(0, DISK_SIZE - 1) for i in range(1000)]
        print("Doing random stuff")
    else:
        headLocation = int(args[1])
        headDirection = args[2].upper() == "H"
        if args[3].upper().startswith("R"):
            destinations = [random.randint(0, DISK_SIZE - 1) for i in range(int(args[3][1:]))]
        else:
            destinations = [int(args[x]) for x in range(3, len(args))]

    methods = {
        "FCFS": FCFSSeekMethod(headLocation, headDirection, destinations),
        "SSTF": SSTFSeekMethod(headLocation, headDirection, destinations),
        "SCAN": SCANSeekMethod(headLocation, headDirection, destinations),
        "C-SCAN": CSCANSeekMethod(headLocation, headDirection, destinations),
        "LOOK": LOOKSeekMethod(headLocation, headDirection, destinations),
        "C-LOOK": CLOOKSeekMethod(headLocation, headDirection, destinations)
    }

    for method in methods.values():
        method.execute()

    print("== Service History ==")

    for name, method in methods.items():
        print(name, method.seeked)

    print("== Service Stats ==")
    for name, method in methods.items():
        print(name, method.headDirectionChanges, method.headCylinderCount)

class SeekMethod:
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int]):
        self.headLocation = headLocation
        self.headDirection = headDirection
        self.destinations = destinations.copy()
        self.seeked = []
        self.headDirectionChanges = 0
        self.headCylinderCount = 1
    
    def execute(self):
        while self.destinations:
            nextCylinderIndex = self.getNextCylinder()
            nextCylinder = self.destinations[nextCylinderIndex]
            if nextCylinder == self.headLocation:
                del self.destinations[nextCylinderIndex]
                continue

            oldHeadDirection = self.headDirection
            self.headDirection = nextCylinder > self.headLocation
            if self.headDirection != oldHeadDirection:
                self.headDirectionChanges += 1

            self.headCylinderCount += abs(self.headLocation - nextCylinder)
            self.seeked.append(nextCylinder)
            self.headLocation = nextCylinder
            del self.destinations[nextCylinderIndex]

    @abc.abstractmethod
    def getNextCylinder(self):
        pass

class SSTFSeekMethod(SeekMethod):
    def getNextCylinder(self):
        minDist = -1
        minIndex = -1

        for index, cylinder in enumerate(self.destinations):
            dist = abs(cylinder - self.headLocation)
            if dist < minDist or minDist == -1:
                minDist = dist
                minIndex = index

        return minIndex
        
class FCFSSeekMethod(SeekMethod):
    def getNextCylinder(self):
        return 0

class CGeneralSeekMethod(SeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int], min: int, max: int):
        super().__init__(headLocation, headDirection, destinations.copy())
        self.destinations.sort()
        self.min = min
        self.max = max

    def getNextCylinder(self):
        if not self.headDirection:
            # Since we know all the cylinders are available at once,
            # we know we had to have been initialized with the cylinder head
            # moving towards lower cylinders.
            self.headCylinderCount += self.headLocation - self.min - 1
            self.headLocation = self.min
            # self.headDirectionChanges += 1
            self.headDirection = True
            # We've "set" the head to be at 0, now return the lowest-next cylinder
            return 0
        
        for index, cylinder in enumerate(self.destinations):
            if cylinder < self.headLocation:
                continue
            return index
        # We've reached the highest but still have remaining destinations,
        # need to go all the way to DISK_SIZE - 1 and then back to 0 to re-CSCAN
        self.headCylinderCount += self.max - self.headLocation - 2
        self.headCylinderCount += self.max - self.min - 1 # Yikes! That's expensive!
        self.headDirectionChanges += 1
        self.headDirection = True
        return 0

class GeneralSeekMethod(SeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int], min: int, max: int):
        super().__init__(headLocation, headDirection, destinations.copy())
        self.min = min
        self.max = max
        self.destinations.sort()

    def getNextCylinder(self):
        if self.headDirection: # Going towards higher values
            for index, cylinder in enumerate(self.destinations):
                if cylinder > self.headLocation:
                    return index
            # We've reached the highest cylinder, keep going till we get to DISK_SIZE - 1 to turn back around
            self.headCylinderCount += self.max - self.headLocation
            self.headLocation = self.max
            self.headDirectionChanges += 1
            self.headDirection = False
            return len(self.destinations) - 1
        else:
            for index, cylinder in enumerate(self.destinations[::-1]):
                if cylinder < self.headLocation:
                    return index
            # We've reached the lowest cylinder, keep going until we get to 0 to turn back around
            self.headCylinderCount += self.headLocation - self.min
            self.headLocation = self.min
            self.headDirectionChanges += 1
            self.headDirection = True
            return 0

class CSCANSeekMethod(CGeneralSeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int]):
        super().__init__(headLocation, headDirection, destinations.copy(), 0, DISK_SIZE - 1)

class SCANSeekMethod(GeneralSeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int]):
        super().__init__(headLocation, headDirection, destinations.copy(), 0, DISK_SIZE - 1)

class CLOOKSeekMethod(CGeneralSeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int]):
        sorted = destinations.copy()
        sorted.sort()
        super().__init__(headLocation, headDirection, destinations.copy(), sorted[0], sorted[-1])

class LOOKSeekMethod(GeneralSeekMethod):
    def __init__(self, headLocation: int, headDirection: bool, destinations: List[int]):
        sorted = destinations.copy()
        sorted.sort()
        super().__init__(headLocation, headDirection, destinations.copy(), sorted[0], sorted[-1])

if __name__ == "__main__":
    main()
