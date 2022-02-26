//Name: Samuel Kheysin
//Date: 6/10/21
//Description: Create an island/continent out of a method known as a "Particle Roll" Algorithm
//Usage: <exe> [-f] [outputfile]

#include <iostream>    

using namespace std;    

int** makeParticleMap(int width, int height,
    int windowX, int windowY, int winWidth, int winHeight,
    int numParticles, int maxLife);
bool moveExists(int** map, int width, int height, int x, int y);
int findMax(int** map, int width, int height);
void normalizeMap(int** map, int width, int height, int maxVal);
char** polishMap(int** map, int width, int height, int waterLine);

int main()
{
    // Prompt for values
    int width, height, windowX, windowY, winWidth, winHeight, numParticles, maxLife, waterLine;
    cout << "Welcome to Sam's CSE240 island generator!" << endl << endl;
    // Width
    cout << endl;
    cout << "Enter grid width:";
    cin >> width;
    if (width < 0)
    {
        cout << "Width invalid" << endl;
        return 1;
    }
    cout << endl;
    // Height
    cout << "Enter grid height:";
    cin >> height;
    if (height < 0)
    {
        cout << "Height invalid" << endl;
        return 1;
    }
    cout << endl;
    // windowX
    cout << "Enter drop-window x-coordinate (0 - <width>):";
    cin >> windowX;
    if (windowX < 0 || windowX >= width)
    {
        cout << "Drop-window x-coordinate invalid" << endl;
        return 1;
    }
    cout << endl;
    // windowY
    cout << "Enter drop-window y-coordinate (0 - <height>):";
    cin >> windowY;
    if (windowY < 0 || windowY >= height)
    {
        cout << "Drop-window y-coordinate invalid" << endl;
        return 1;
    }
    cout << endl;
    // winWidth
    cout << "Enter drop-window width (>= 1):";
    cin >> winWidth;
    if (winWidth < 1 || windowX + winWidth > width)
    {
        cout << "Drop-window width invalid" << endl;
        return 1;
    }
    cout << endl;
    // winHeight
    cout << "Enter drop-window height (>= 1):";
    cin >> winHeight;
    if (winHeight < 1 || windowY + winHeight > height)
    {
        cout << "Drop-window height invalid" << endl;
        return 1;
    }
    cout << endl;
    // numParticles
    cout << "Enter number of particles to drop:";
    cin >> numParticles;
    if (numParticles <= 0)
    {
        cout << "Number of particles invalid" << endl;
        return 1;
    }
    cout << endl;
    // maxLife
    cout << "Enter max life of particles:";
    cin >> maxLife;
    if (maxLife <= 0)
    {
        cout << "Max life of particles invalid" << endl;
        return 1;
    }
    cout << endl;
    // waterLine
    cout << "Enter value for waterline (40-200):";
    cin >> waterLine;
    if (waterLine < 40 || waterLine > 200)
    {
        cout << "Waterline invalid" << endl;
        return 1;
    }
    cout << endl << endl;

    // Create island
    int** map = makeParticleMap(width, height, windowX, windowY, winWidth, winHeight, numParticles, maxLife);

    // Get max
    int maxVal = findMax(map, width, height);

    // Normalize
    normalizeMap(map, width, height, maxVal);

    // Polish island
    char** charMap = polishMap(map, width, height, waterLine);

    // Print island
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << charMap[j][i];
        }
        cout << endl;
    }

    // Delete arrays
    for (int i = 0; i < width; i++)
    {
        delete[] map[i];
        delete[] charMap[i];
    }
    delete[] map;
    delete[] charMap;

    return 0;
}

int** makeParticleMap(int width, int height,
    int windowX, int windowY, int winWidth, int winHeight,
    int numParticles, int maxLife)
{
    // Create 2d array
    int** map = new int* [width];
    for (int i = 0; i < width; i++)
        map[i] = new int[height];
    // Initialize
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            map[i][j] = 0;
    // Iterate for every particle
    for (int particle = 0; particle < numParticles; particle++)
    {
        // Drop particle
        int x = 0, y = 0;
        while (true)
        {
            // Generate a random point
            int dx = rand() % winWidth;
            int dy = rand() % winHeight;
            x = windowX + dx;
            y = windowY + dy;
            // Check
            if (moveExists(map, width, height, x, y))
                break;
        }
        // Increase value of initial position
        map[x][y]++;
        // Iterate until life becomes 0
        for (int life = maxLife; life > 0; life--)
        {
            // If further movement is not possible, go to next particle
            if (!moveExists(map, width, height, x, y))
                break;
            // Movement steps in 8 directions
            int dx[] = { 1, 1, 1, 0, 0, -1, -1, -1 };
            int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
            // Move
            while (true)
            {
                // Generate a random move
                int dir = rand() % 8;
                int x2 = x + dx[dir];
                int y2 = y + dy[dir];
                // Check coordinates
                if (x2 >= 0 && y2 >= 0 && x2 < width && y2 < height)
                {
                    // Check values
                    if (map[x2][y2] <= map[x][y])
                    {
                        // Move
                        x = x2;
                        y = y2;
                        // Increase value
                        map[x][y]++;
                        // Go to next iteration
                        break;
                    }
                }
            }
        }
    }
    return map;
}

bool moveExists(int** map, int width, int height, int x, int y)
{
    // Movement steps in 8 directions
    int dx[] = { 1, 1, 1, 0, 0, -1, -1, -1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    // Check 8 neighbors
    for (int i = 0; i < 8; i++)
    {
        // Get neighbor coordinates
        int x2 = x + dx[i];
        int y2 = y + dy[i];
        // Check coordinates
        if (x2 >= 0 && y2 >= 0 && x2 < width && y2 < height)
        {
            // Check values
            if (map[x2][y2] <= map[x][y])
                // Found a valid move
                return true;
        }
    }
    // Valid move not found
    return false;
}

int findMax(int** map, int width, int height)
{
    // Find maximum cell value
    int max = 0;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            if (map[i][j] > max)
                max = map[i][j];
    return max;
}

void normalizeMap(int** map, int width, int height, int maxVal)
{
    // Divide each cell by maxVal and multiply by 255
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            map[i][j] = (int)(map[i][j] / (float)maxVal * 255.0 + 0.5f);
}

char** polishMap(int** map, int width, int height, int waterLine)
{
    // Create 2d array
    char** charMap = new char* [width];
    for (int i = 0; i < width; i++)
        charMap[i] = new char[height];
    // Polish
    int landZone = 255 - waterLine;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Deep water
            if (map[i][j] < waterLine / 2.0)
                charMap[i][j] = '#';
            // Shallow water
            else if (map[i][j] <= waterLine)
                charMap[i][j] = '~';
            // Coast/beach
            else if (map[i][j] < waterLine + (int)(landZone * 0.15 + 0.5))
                charMap[i][j] = '.';
            // Plains/grass
            else if (map[i][j] < waterLine + (int)(landZone * 0.4 + 0.5))
                charMap[i][j] = '_';
            // Forest
            else if (map[i][j] < waterLine + (int)(landZone * 0.8 + 0.5))
                charMap[i][j] = '*';
            // Mountain
            else
                charMap[i][j] = '^';
        }
    }
    return charMap;
}