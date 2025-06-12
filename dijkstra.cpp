#include <cstdio>

// initializing a very large number
const int INF = 1 << 30;

// grid position and equality operator to compare positions
struct Position {
    int col, row;
    bool operator==(const Position& other) const {
        return col == other.col && row == other.row;
    }
};

// representation of a single lift
struct Lift {
    Position start;
    Position end;
    int travelTime; // how long the lift takes
    int interval; // in what intervals it is going
};

// state representation
struct State {
    int row, col;
    int time;
};

// list of all lifts
struct LiftList {
    Lift* lifts;
    int count;
};

// custom priority queue
struct MinHeap {
    State* heap;
    int capacity;
    int size;
    int** positionMap; // positions of grid cells in the heap for fast access
    int width, height;

    // implementation of binary tree using for navigating
    static int parent(int i) { return (i - 1) / 2; }
    static int leftChild(int i) { return 2 * i + 1; }
    static int rightChild(int i) { return 2 * i + 2; }

    // swaps two elements in the heap and updates the positionMap
    void swap(int i, int j) {
        State temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
        positionMap[heap[i].row][heap[i].col] = i;
        positionMap[heap[j].row][heap[j].col] = j;
    }

    // moves the node upward if it is smaller than its parent
    void heapifyUp(int i) {
        while (i > 0 && heap[i].time < heap[parent(i)].time) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    // moves the node downward to its correct position
    void heapifyDown(int i) {
        int minIdx = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < size && heap[left].time < heap[minIdx].time)
            minIdx = left;
        if (right < size && heap[right].time < heap[minIdx].time)
            minIdx = right;

        if (minIdx != i) {
            swap(i, minIdx);
            heapifyDown(minIdx);
        }
    }

    // initializing heap
    MinHeap(int w, int h) {
        width = w;
        height = h;
        capacity = w * h;
        size = 0;
        heap = new State[capacity];
        positionMap = new int*[height];
        for (int i = 0; i < height; ++i) {
            positionMap[i] = new int[width];
            for (int j = 0; j < width; ++j)
                positionMap[i][j] = -1;
        }
    }

    // destructor
    ~MinHeap() {
        delete[] heap;
        for (int i = 0; i < height; ++i)
            delete[] positionMap[i];
        delete[] positionMap;
    }

    // pushes a state to the heap
    void push(int row, int col, int time) {
        int& pos = positionMap[row][col];
        if (pos != -1) { // check if already in the heap
            if (time < heap[pos].time) { // if the new time is smaller than the existing time in the heap, update it
                heap[pos].time = time;
                heapifyUp(pos);
            }
            return;
        }

        // if a new position
        heap[size] = {row, col, time};
        pos = size;
        heapifyUp(size);
        size++;
    }

    // return top element
    State top() const { return heap[0]; }

    void pop() {
        if (size <= 0) return; // if empty return
        // remove top element and mark as not existent (-1)
        positionMap[heap[0].row][heap[0].col] = -1;
        heap[0] = heap[size - 1];
        size--;
        // if there are elements restore order with heapifyDown
        if (size > 0) {
            positionMap[heap[0].row][heap[0].col] = 0;
            heapifyDown(0);
        }
    }

    bool empty() const { return size == 0; }
};

int findShortestRoute(int width, int height, Position start, Position dest,
                     LiftList** liftMap, int** terrain) {
    // initialize array with INF values
    int** distance = new int*[height];
    for (int i = 0; i < height; ++i) {
        distance[i] = new int[width];
        for (int j = 0; j < width; ++j)
            distance[i][j] = INF;
    }

    // initialize priority queue and insert state
    MinHeap pq(width, height);
    distance[start.row][start.col] = 0;
    pq.push(start.row, start.col, 0);

    // vectors for moving
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    // main dijkstra loop
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (current.time > distance[current.row][current.col]) continue; // skip outdated entries
        // check if destination is reached
        if (current.row == dest.row && current.col == dest.col) {
            int result = current.time;
            for (int i = 0; i < height; ++i) delete[] distance[i];
            delete[] distance;
            return result;
        }
        // all lifts available at current cell
        LiftList& liftList = liftMap[current.row][current.col];
        // loop for checking all available lifts
        for (int i = 0; i < liftList.count; ++i) {
            Lift& lift = liftList.lifts[i];
            int waitTime = 0;
            // calculation wait time
            if (lift.interval > 0) {
                int mod = current.time % lift.interval;
                if (mod > 0) waitTime = lift.interval - mod;
            }
            // total time after using lift
            int newTime = current.time + waitTime + lift.travelTime;
            // compare time and update if better
            if (newTime < distance[lift.end.row][lift.end.col]) {
                distance[lift.end.row][lift.end.col] = newTime;
                pq.push(lift.end.row, lift.end.col, newTime);
            }
        }
        //moving without lifts
        for (int d = 0; d < 4; ++d) {
            int nr = current.row + dr[d];
            int nc = current.col + dc[d];
            if (nr < 0 || nr >= height || nc < 0 || nc >= width) continue;
            int moveTime = (terrain[nr][nc] > terrain[current.row][current.col]) ?
                           (terrain[nr][nc] - terrain[current.row][current.col] + 1) : 1;
            int newTime = current.time + moveTime;
            if (newTime < distance[nr][nc]) {
                distance[nr][nc] = newTime;
                pq.push(nr, nc, newTime);
            }
        }
    }
    // freeing memory
    for (int i = 0; i < height; ++i) delete[] distance[i];
    delete[] distance;
    return -1;
}

int main() {
    // initialize and read starting values
    int width, height, liftCount;
    Position start, dest;

    scanf("%d %d %d %d %d %d %d", &width, &height, &start.col, &start.row, &dest.col, &dest.row, &liftCount);

    // initializing empty lift list
    Lift* lifts = nullptr;
    LiftList** liftMap = new LiftList*[height];
    for (int i = 0; i < height; ++i) {
        liftMap[i] = new LiftList[width];
        for (int j = 0; j < width; ++j) {
            liftMap[i][j].lifts = nullptr;
            liftMap[i][j].count = 0;
        }
    }

    // initializing lifts
    if (liftCount > 0) {
        lifts = new Lift[liftCount];
        int** liftCounts = new int*[height];
        for (int i = 0; i < height; ++i) {
            liftCounts[i] = new int[width];
            for (int j = 0; j < width; ++j)
                liftCounts[i][j] = 0;
        }

        for (int i = 0; i < liftCount; ++i) {
            scanf("%d %d %d %d %d %d", &lifts[i].start.col, &lifts[i].start.row,
                     &lifts[i].end.col, &lifts[i].end.row,
                     &lifts[i].travelTime, &lifts[i].interval);
            liftCounts[lifts[i].start.row][lifts[i].start.col]++;
        }

        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (liftCounts[i][j] > 0)
                    liftMap[i][j].lifts = new Lift[liftCounts[i][j]];

        for (int i = 0; i < liftCount; ++i) {
            int r = lifts[i].start.row;
            int c = lifts[i].start.col;
            liftMap[r][c].lifts[liftMap[r][c].count++] = lifts[i];
        }

        for (int i = 0; i < height; ++i) delete[] liftCounts[i];
        delete[] liftCounts;
    }

    // initialize terrain
    int** terrain = new int*[height];
    for (int i = 0; i < height; ++i) {
        terrain[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            scanf("%d", &terrain[i][j]);
        }
    }

    // main function
    int result = findShortestRoute(width, height, start, dest, liftMap, terrain);
    printf("%d\n", result);

    //freeing memory
    if (lifts) delete[] lifts;
    for (int i = 0; i < height; ++i) delete[] terrain[i];
    delete[] terrain;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) delete[] liftMap[i][j].lifts;
        delete[] liftMap[i];
    }
    delete[] liftMap;
}
