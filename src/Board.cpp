#include <Board.h>

Board::Board()
{
    // Init pixels
    for(int i = 0; i < BOARD_WIDTH; ++i)
    {
        for(int j = 0; j < BOARD_HEIGHT; ++j)
        {
            pixels[i][j] = Pixel::OFF;
        }
    }
}
 
void Board::addTetrominoe(Tetrominoe::Type type)
{
    currentTetrominoe = new Tetrominoe(type);
    currentTetrominoe->x = (SCREEN_WIDTH/2)-1;
}

bool Board::hasTetrominoe()
{
    return currentTetrominoe != NULL;
}

void Board::sealTetrominoe()
{
    if(currentTetrominoe == NULL) return;
    Pixel pixel = (Pixel)currentTetrominoe->type;
    for(int i = 0 ; i < TETROMINOE_SIZE; i++)
    {
        for(int j = 0 ; j < TETROMINOE_SIZE; j++)
        {
            if(currentTetrominoe->getShape(i,j))
            {
                pixels[currentTetrominoe->x+i][currentTetrominoe->y+j] = pixel;
            }
        }
    }
    delete currentTetrominoe;
    currentTetrominoe = NULL;
}

Board::Collision Board::detectCollision()
{
    if(currentTetrominoe == NULL) return Collision::NONE;
    int x,y;
    for(int i = 0 ; i < TETROMINOE_SIZE; i++)
    {
        for(int j = 0 ; j < TETROMINOE_SIZE; j++)
        {
            if(currentTetrominoe->getShape(i,j))
            {
                x = currentTetrominoe->x+i;
                // Detect border collision
                if((x >= SCREEN_WIDTH)) return Collision::BORDER_RIGHT;
                if(x < 0) return Collision::BORDER_LEFT;
                y = currentTetrominoe->y+j;
                // Detect botton collision
                if(y >= SCREEN_HEIGHT) return Collision::BOTTOM;
                // Dettec colision with other tetrominoes
                if(pixels[x][y] >= 0) return Collision::PIXEL;
            }
        }
    }
    return Collision::NONE;
}

bool Board::checkAndGiveSpace()
{
    Collision collision = detectCollision();
    if(collision == Collision::NONE) return true;
    if(collision == Collision::BORDER_LEFT)
    {   // Try and give some space
        int oldX = currentTetrominoe->x;
        while(collision == Collision::BORDER_LEFT)
        {   
            currentTetrominoe->x++;
            collision = detectCollision();
        }
        if(collision == Collision::NONE)
        {
            return true;
        }
        else
        {
            currentTetrominoe->x = oldX;
        }
    }
    else if(collision == Collision::BORDER_RIGHT)
    {   // Try and give some space
        int oldX = currentTetrominoe->x;
        while(collision == Collision::BORDER_RIGHT)
        {   
            currentTetrominoe->x--;
            collision = detectCollision();
        }
        if(collision == Collision::NONE)
        {
            return true;
        }
        else
        {
            currentTetrominoe->x = oldX;
        }
    }
    return false;
}
 
bool Board::rotateTetrominoeLeft()
{
    if(currentTetrominoe == NULL) return false;
    currentTetrominoe->rotateLeft();
    if(checkAndGiveSpace()) return true;
    // Rotation not possible, cancel it
    currentTetrominoe->rotateRight();
    return false;
}

bool Board::rotateTetrominoeRight()
{
    if(currentTetrominoe == NULL) return false;
    currentTetrominoe->rotateRight();
    if(checkAndGiveSpace()) return true;
    // Rotation not possible, cancel it
    currentTetrominoe->rotateLeft();
    return false;
}

bool Board::moveTetrominoeLeft()
{
    if(currentTetrominoe == NULL) return false;
    currentTetrominoe->x--;
    Serial.println(currentTetrominoe->x);
    if(detectCollision())
    {
        currentTetrominoe->x++;
        return false;
    }
    return true;
}

bool Board::moveTetrominoeRight()
{
    if(currentTetrominoe == NULL) return false;
    currentTetrominoe->x++;
    if(detectCollision())
    {
        currentTetrominoe->x--;
        return false;
    }
    return true;
}

bool Board::moveTetrominoeDown()
{
    if(currentTetrominoe == NULL) return false;
    currentTetrominoe->y++;
    if(detectCollision())
    {
        currentTetrominoe->y--;
        return false;
    }
    return true;
}

void Board::highlightLine(int lineY)
{
    for(int i=0; i < SCREEN_WIDTH ; i++)
    {
        pixels[i][lineY] = Pixel::LINE;
    }
}

void Board::removeHighlightedLines()
{
    for(int i = (SCREEN_HEIGHT-1) ; i >= 0 ; i--)
    {
        if(pixels[0][i] == Pixel::LINE)
        {   // Remove this line
            for(int j = i ;  j > 0 ; j--)
            {   // Move upper lines down
                for(int k = 0 ; k < SCREEN_WIDTH ; k++)
                {   // Copy each pixel of the line
                    pixels[k][j] = pixels[k][j-1];
                }
            }
            i++;
        }
    }
}

bool Board::isLineComplete(int lineY)
{
    for(int i=0; i < SCREEN_WIDTH ; i++)
    {   // Check for empty pixel
        if(pixels[i][lineY] < 0) return false;
    }
    return true;
}

const struct CRGB PIXEL_COLORS[] = { CRGB(0, 255, 255), CRGB(0, 0, 255), CRGB(255, 100, 0), CRGB(255, 255, 0), CRGB(20, 255, 20), CRGB(255, 0, 255), CRGB(255, 0, 0), CRGB(255, 255, 255) };

void Board::render(FastLED_NeoMatrix * ledMatrix)
{
    // First render tetrominoe
    if(currentTetrominoe != NULL)
    {
        currentTetrominoe->render(ledMatrix);
    }
    // The draw the board's pixels
    for(int i = 0 ; i < SCREEN_WIDTH; i++)
    {
        for(int j = 0 ; j < SCREEN_HEIGHT; j++)
        {
            Pixel pixel = pixels[i][j];
            if(pixel >= 0)
            {
                ledMatrix->drawPixel(i,j,PIXEL_COLORS[pixel]);
            }
        }
    }
}

