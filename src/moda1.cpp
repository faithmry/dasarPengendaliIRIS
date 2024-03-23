#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

Point2i point(int x, int y)
{
    return Point2i(100 + x, 700 - y);
}

Mat img(800, 1000, CV_8UC3, Scalar(156, 153, 152));
int samping = 0, tengah = 0, gawang = 0;

void lapangan()
{
    rectangle(img, point(0, 0), point(800, 600), Scalar(59, 173, 97), -1); // court

    rectangle(img, point(0, 200 + (75 / 2)), point(75, 200 - (75 / 2)), Scalar(54, 109, 207), -1);   // robot kiri
    rectangle(img, point(725, 200 + (75 / 2)), point(800, 200 - (75 / 2)), Scalar(207, 97, 54), -1); // robot kanan

    rectangle(img, point(0, 0), point(800, 600), Scalar(255, 255, 255), 4, LINE_8);           // outline
    line(img, point(300, 450), point(300, 0), Scalar(93, 237, 245), 4, LINE_8);               // kiri dari half court
    line(img, point(500, 450), point(500, 0), Scalar(93, 237, 245), 4, LINE_8);               // kanan dari half court
    line(img, point(200, 450), point(600, 450), Scalar(93, 237, 245), 4, LINE_8);             // penalti depan
    line(img, point(200, 600), point(200, 450), Scalar(93, 237, 245), 4, LINE_8);             // penalti kiri
    line(img, point(600, 600), point(600, 450), Scalar(93, 237, 245), 4, LINE_8);             // penalti kanan
    ellipse(img, point(400, 0), Size(100, 100), 0, 180, 180 + 180, Scalar(255, 255, 255), 4); // half court
    ellipse(img, point(0, 600), Size(50, 50), 0, 0, 0 + 90, Scalar(255, 255, 255), 4);        // kiri
    ellipse(img, point(800, 600), Size(50, 50), 0, 180, 180 - 90, Scalar(255, 255, 255), 4);  // kanan
    // circle(img, point(0, 0), 10, Scalar(255, 255, 255), 1);
}

vector<Point2i> titikObstacleSamping = {{point(200, 200)}, {point(200, 300)}, {point(200, 400)}, {point(600, 200)}, {point(600, 300)}, {point(600, 400)}};
vector<Point2i> titikObstacleTengah = {{point(400, 300)}, {point(400, 400)}};
vector<Point2i> titikObstacleGawang = {{point(400, 600)}, {point(500, 600)}, {point(300, 600)}};

void drawTitikObstacle()
{
    for (auto i : titikObstacleSamping)
    {
        // ellipse(image, i, Size(3, 3), 0, 0, 360, cv::Scalar(20, 20, 20), -1);
        circle(img, i, 2, Scalar(0, 0, 0), 2);
    }

    for (auto i : titikObstacleTengah)
    {
        circle(img, i, 2, Scalar(0, 0, 0), 2);
    }

    for (auto i : titikObstacleGawang)
    {
        circle(img, i, 2, Scalar(0, 0, 0), 2);
    }
}

vector<Rect2i> obstacleSamping = {
    {point(200 - 26, 200 - 26), {point(200 + 26, 200 + 26)}},
    {point(200 - 26, 300 - 26), {point(200 + 26, 300 + 26)}},
    {point(200 - 26, 400 - 26), {point(200 + 26, 400 + 26)}},

    {point(600 - 26, 200 - 26), {point(600 + 26, 200 + 26)}},
    {point(600 - 26, 300 - 26), {point(600 + 26, 300 + 26)}},
    {point(600 - 26, 400 - 26), {point(600 + 26, 400 + 26)}},
};

vector<Rect2i> obstacleTengah = {
    {point(400 - 26, 400 - 26), {point(400 + 26, 400 + 26)}},
    {point(400 - 26, 300 - 26), {point(400 + 26, 300 + 26)}},
};

vector<Rect2i> obstacleGawang = {
    {point(400 - 26, 600 - 26), {point(400 + 26, 600 + 26)}},
    {point(500 - 26, 600 - 26), {point(500 + 26, 600 + 26)}},
    {point(300 - 26, 600 - 26), {point(300 + 26, 600 + 26)}},
};

void drawObstacle()
{
    rectangle(img, obstacleSamping[samping], Scalar(0, 0, 0), -1);
    rectangle(img, obstacleTengah[tengah], Scalar(0, 0, 0), -1);
    rectangle(img, obstacleGawang[gawang], Scalar(0, 0, 0), -1);
}

// posisi robot kick off kanan

Point startPoinBiru = point(800 - 75 / 2, 200);
Point startPoinMerah = point(75 / 2, 200);
Point startPoinBola = point(400, 0);

vector<vector<vector<Point2i>>> jalanBiru = {
    {
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 300), point(580, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 300), point(580, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 300), point(580, 400)},
        //
        {startPoinBiru, startPoinBola, point(700, 100), point(730, 300), point(610, 300), point(610, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(730, 230), point(700, 400), point(610, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 250), point(580, 300)},
    },
    {
        {startPoinBiru, startPoinBola, point(600, 100), point(700, 170), point(580, 200), point(580, 350)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 250), point(580, 370)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 250), point(580, 370)},
        //
        {startPoinBiru, startPoinBola, point(700, 100), point(680, 280), point(680, 360), point(610, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(670, 200), point(700, 370), point(610, 400)},
        {startPoinBiru, startPoinBola, point(600, 100), point(610, 170), point(620, 250), point(580, 300)},
    }};

vector<vector<vector<Point2i>>> jalanMerah = {
    {
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(100, 300), point(100, 300)},
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(200, 200), point(200, 200)},
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(200, 200), point(200, 200)},
        //
        {startPoinMerah, point(100, 100), point(200, 230), point(200, 350), point(220, 250), point(220, 250)},
        {startPoinMerah, point(100, 100), point(200, 100), point(200, 150), point(200, 250), point(200, 250)},
        {startPoinMerah, point(100, 100), point(200, 100), point(200, 150), point(200, 250), point(200, 250)},
    },
    {
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(100, 300), point(100, 300)},
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(200, 200), point(200, 200)},
        {startPoinMerah, point(100, 90), point(220, 100), point(120, 110), point(200, 200), point(200, 200)},
        //
        {startPoinMerah, point(100, 100), point(200, 150), point(200, 200), point(220, 350), point(220, 350)},
        {startPoinMerah, point(100, 100), point(200, 200), point(200, 250), point(200, 400), point(200, 400)},
        {startPoinMerah, point(100, 100), point(200, 100), point(200, 150), point(200, 250), point(200, 250)},
    }

};

// goal kiri

void strategi()
{
    for (int i = 0; i < jalanBiru[tengah][samping].size() - 1; i++)
    {
        line(img, jalanBiru[tengah][samping][i], jalanBiru[tengah][samping][i + 1], Scalar((i + 1) * 50, 0, 0), 2);
    }

    for (int i = 0; i < jalanMerah[tengah][samping].size() - 1; i++)
    {
        line(img, jalanMerah[tengah][samping][i], jalanMerah[tengah][samping][i + 1], Scalar(0, 0, (i + 1) * 50), 2);
    }

    for (int i = 1; i < jalanMerah[tengah][samping].size() - 1; i++)
    {
        line(img, jalanMerah[tengah][samping][i], jalanBiru[tengah][samping][i], Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);

        if (i == jalanMerah[tengah][samping].size() - 2)
        {
            switch (gawang)
            {
            case 0:
                line(img, jalanBiru[tengah][samping][i + 1], point(450, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
                break;
            case 1:
                line(img, jalanBiru[tengah][samping][i + 1], point(450, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
                break;
            case 2:
                line(img, jalanBiru[tengah][samping][i + 1], point(400, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
                break;
            }
        }
    }
}

int main(int argc, char **argv)
{
    tengah = stoi(argv[1]) > 1 ? 1 : stoi(argv[1]);
    samping = stoi(argv[2]) > 5 ? 5 : stoi(argv[2]);
    gawang = stoi(argv[3]) > 2 ? 2 : stoi(argv[3]);
    lapangan();
    drawTitikObstacle();
    drawObstacle();
    strategi();
    imshow("Output", img);
    waitKey(0);
    return 0;
}
