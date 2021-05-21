#include <SFML/Graphics.hpp>
#include <iostream>
#include "Kruskal.h"
using namespace sf;
class MainWindow
{
private:
    RenderWindow window;

    Kruskal kruskal;

    const float PI = 3.141592;

    const float xOffset = 13.f;
    const float yOffset = 13.f;
    const float minDistance = 60.f;

    Clock timer, kruskTimer;

    bool isSpaceToggled = false;

    Font font;

    void initWindow()
    {
        new(&window) RenderWindow(VideoMode(800, 800), "Kruskal algorithm");
        while (window.isOpen())
        {
            reloadGraphButton();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
            }
            window.clear(Color::White);
            drawGraph();

            executeAlgorithmButton();

            if (isSpaceToggled)
            {
                drawAlgorithm();
            }
            drawHintText();
            window.display();
        }
    }

    void reloadGraphButton()
    {
        if (Keyboard::isKeyPressed(Keyboard::R))
        {
            kruskTimer.restart();
            isSpaceToggled = false;
            kruskal.reload();
            cout << "Time: " << kruskTimer.getElapsedTime().asMicroseconds() << endl;
            timer.restart();
        }
    }

    void executeAlgorithmButton()
    {
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            isSpaceToggled = true;
            timer.restart();
        }
    }

    void drawAlgorithm()
    {
        auto minSpanTree = kruskal.getTree();
        int a = 0;
        for (auto it = minSpanTree.begin(); it != minSpanTree.end(); it++, a++)
        {
            if (timer.getElapsedTime().asSeconds() > a)
            {
                drawOneLine(it, Color::Magenta);
            }
        }
    }

    void drawGraph()
    {
        auto graph = kruskal.getGraph();
        drawPoints(graph);
        drawLines(graph, Color::Black);
    }
    void drawPoints(std::multimap<int, std::pair<Point, Point>>& graph)
    {
        for (auto it = graph.begin(); it != graph.end(); it++)
        {
            if (isLowDistance(graph, it))
            {
                kruskal.reload();
            }
            CircleShape circle(20.f);

            Text text;
            if (!font.loadFromFile("Roboto-Regular.ttf"))
            {
                window.close();
            }
            text.setCharacterSize(28);
            text.setFont(font);
            text.setFillColor(Color::Green);
            

            circle.setFillColor(Color::Black);

            circle.setPosition(it->second.first.x, it->second.first.y);
            text.setPosition(it->second.first.x + xOffset, it->second.first.y);
            text.setString(it->second.first.name);
            window.draw(circle);
            window.draw(text);

            circle.setPosition(it->second.second.x, it->second.second.y);
            text.setPosition(it->second.second.x + xOffset, it->second.second.y);
            text.setString(it->second.second.name);
            window.draw(circle);
            window.draw(text);
        }
    }

    bool isLowDistance(std::multimap<int, std::pair<Point, Point>>& graph, std::multimap<int, std::pair<Point, Point>>::iterator curEdge)
    {
        bool isLowDist = false;
        for (auto it = graph.begin(); it != graph.end(); it++)
        {
            if (curEdge->second.first.x != it->second.first.x)
            {
                if (Distance(curEdge->second.first, it->second.first) < minDistance)
                {
                    isLowDist = true;
                }
            } 
        }
        return isLowDist;
    }

    void drawLines(std::multimap<int, std::pair<Point, Point>>& graph, Color color)
    {
        if (!font.loadFromFile("Roboto-Regular.ttf"))
        {
            window.close();
        }
        for (auto it = graph.begin(); it != graph.end(); it++)
        {
            drawOneLine(it, color);
        }
    }

    Vector2f getLineCenter(std::multimap<int, std::pair<Point, Point>>::iterator& it)
    {
        int xCenter = (it->second.second.x - it->second.first.x)/2;
        int yCenter = (it->second.second.y - it->second.first.y)/2;
        xCenter += it->second.first.x;
        yCenter += it->second.first.y;
        return Vector2f(xCenter + xOffset, yCenter + yOffset);
    }

    void drawOneLine(std::multimap<int, std::pair<Point, Point>>::iterator& it, Color color)
    {
        Text text;
        text.setCharacterSize(28);
        text.setFont(font);
        text.setFillColor(Color::Blue);

        RectangleShape line(Vector2f(Distance(it->second.first, it->second.second), 2.f));
        line.setPosition(it->second.first.x + xOffset, it->second.first.y + yOffset);
        if (isNegativeOffset(it))
        {
            line.setPosition(it->second.second.x + xOffset, it->second.second.y + yOffset);
        }
        line.setFillColor(color);
        line.rotate(getLineRotation(it->second));
        window.draw(line);
        text.setPosition(getLineCenter(it));
        string tmp = to_string(it->first);
        text.setString(tmp);
        window.draw(text);
    }

    void drawHintText()
    {
        if (!font.loadFromFile("Roboto-Regular.ttf"))
        {
            window.close();
        }

        Text text;
        text.setCharacterSize(28);
        text.setFont(font);
        text.setFillColor(Color::Black);

        text.setPosition(130, 750);
        text.setString("Press SPACE to start algorithm, R - to reload");
        window.draw(text);
    }

    bool isNegativeOffset(std::multimap<int, std::pair<Point, Point>>::iterator& it)
    {
        return it->second.second.x - it->second.first.x < 0;
    }

    float getLineRotation(pair<Point, Point> points)
    {
        float x = points.second.x - points.first.x;
        float y = points.second.y - points.first.y;
        return ( atan(y / x) * 180.f) / PI;
    }

public:
	MainWindow()
	{
        initWindow();
	}
};