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

// void obstacles()
// {
//     rectangle(img, point(174, 200 + 26), point(226, 200 - 26), Scalar(0, 0, 0), -1); // 1
//     rectangle(img, point(174, 300 + 26), point(226, 300 - 26), Scalar(0, 0, 0), -1); // 2
//     rectangle(img, point(174, 400 + 26), point(226, 400 - 26), Scalar(0, 0, 0), -1); // 3
//     rectangle(img, point(574, 200 + 26), point(626, 200 - 26), Scalar(0, 0, 0), -1); // 4
//     rectangle(img, point(574, 300 + 26), point(626, 300 - 26), Scalar(0, 0, 0), -1); // 5
//     rectangle(img, point(574, 400 + 26), point(626, 400 - 26), Scalar(0, 0, 0), -1); // 6
//     rectangle(img, point(374, 300 + 26), point(426, 300 - 26), Scalar(0, 0, 0), -1); // 7
//     rectangle(img, point(374, 400 + 26), point(426, 400 - 26), Scalar(0, 0, 0), -1); // 8
//     rectangle(img, point(374, 600 + 26), point(426, 600 - 26), Scalar(0, 0, 0), -1); // keeper
// }

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

// posisi robot kick off kiri

Point startPoinBiru = point(800 - 75 / 2, 200);
Point startPoinMerah = point(75 / 2, 200);
Point startPoinBola = point(400, 0);

// vector<vector<vector<Point2i>>> jalanBiru = {
//     {
//         {startPoinBiru, point(700, 100), point(600, 230), point(600, 350), point(580, 250), point(580, 250)},
//         {startPoinBiru, point(700, 100), point(600, 100), point(600, 150), point(600, 250), point(600, 250)},
//         {startPoinBiru, point(700, 100), point(600, 100), point(600, 150), point(600, 250), point(600, 250)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(700, 300), point(700, 300)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//         // {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//     },
//     {
//         {startPoinBiru, point(700, 100), point(600, 150), point(600, 200), point(580, 350), point(580, 350)},
//         {startPoinBiru, point(700, 100), point(600, 200), point(600, 250), point(600, 400), point(600, 400)},
//         {startPoinBiru, point(700, 100), point(600, 100), point(600, 150), point(600, 250), point(600, 250)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(700, 300), point(700, 300)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//         {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//         // {startPoinBiru, point(700, 90), point(580, 100), point(680, 110), point(600, 200), point(600, 200)},
//     }
// };

// vector<vector<vector<Point2i>>> jalanMerah = {
//     {
//         {startPoinMerah, startPoinBola, point(100, 100), point(70, 300), point(190, 300), point(190, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(70, 230), point(100, 400), point(190, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 250), point(220, 300)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 300), point(220, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 300), point(220, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 300), point(220, 400)},
//         // {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 300), point(220, 400)},
//     },
//     {
//         {startPoinMerah, startPoinBola, point(100, 100), point(120, 280), point(120, 360), point(190, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(130, 200), point(100, 370), point(190, 400)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 250), point(220, 300)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(100, 170), point(220, 200), point(220, 350)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 250), point(220, 370)},
//         {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 250), point(220, 370)},
//         // {startPoinMerah, startPoinBola, point(200, 100), point(190, 170), point(180, 300), point(220, 400)},

//     }
// };

// posisi robot kick off kanan

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

Point startPoinBolaKiri = point(0, 600);
Point startPoinBolaKanan = point(800, 600);

// posisi robot corner kanan

// vector<vector<vector<Point2i>>> jalanBiru = {
//     {
//         {startPoinBiru, startPoinBolaKanan, point(550, 150), point(550, 150), point(550, 150), point(550, 150)},
//         {startPoinBiru, startPoinBolaKanan, point(550, 200), point(550, 200), point(550, 200), point(550, 200)},
//         {startPoinBiru, startPoinBolaKanan, point(550, 200), point(550, 200), point(550, 200), point(550, 200)},
//         {startPoinBiru, startPoinBolaKanan, point(550, 265), point(550, 265), point(550, 265), point(550, 265)},
//         {startPoinBiru, startPoinBolaKanan, point(650, 510), point(650, 510), point(650, 510), point(650, 510)},
//         {startPoinBiru, startPoinBolaKanan, point(550, 150), point(550, 150), point(550, 150), point(550, 150)},
//     },
//     {
//         {startPoinBiru, startPoinBolaKanan, point(650, 300), point(650, 300), point(650, 300), point(650, 300)},
//         {startPoinBiru, startPoinBolaKanan, point(650, 300), point(650, 300), point(650, 300), point(650, 300)},
//         {startPoinBiru, startPoinBolaKanan, point(580, 400), point(580, 400), point(580, 400), point(580, 400)},
//         {startPoinBiru, startPoinBolaKanan, point(580, 400), point(580, 400), point(580, 400), point(580, 400)},
//         {startPoinBiru, startPoinBolaKanan, point(580, 400), point(580, 400), point(580, 400), point(580, 400)},
//         {startPoinBiru, startPoinBolaKanan, point(650, 500), point(650, 500), point(650, 500), point(650, 500)},
//     }};

// vector<vector<vector<Point2i>>> jalanMerah = {
//     {
//         {startPoinMerah, point(150, 80), point(100, 300), point(240, 420), point(240, 420), point(240, 420)}, // 450 mentok, titik y maks. 420
//         {startPoinMerah, point(200, 200), point(265, 200), point(265, 420), point(265, 420), point(265, 420)}, // 300 mentok, titik x maks. 265
//         {startPoinMerah, point(200, 200), point(265, 200), point(265, 420), point(265, 420), point(265, 420)}, // 300 mentok, titik x maks. 265
//         {startPoinMerah, point(200, 200), point(265, 200), point(245, 420), point(245, 420), point(245, 420)}, // 300 mentok, titik x maks. 265
//         {startPoinMerah, point(200, 200), point(265, 220), point(245, 420), point(245, 420), point(245, 420)}, // 300 mentok, titik x maks. 265
//         {startPoinMerah, point(200, 200), point(265, 220), point(265, 420), point(265, 420), point(265, 420)}, // 300 mentok, titik x maks. 265
//     },
//     {
//         {startPoinMerah, point(265, 80), point(265, 150), point(265, 420), point(265, 420), point(265, 420)}, // 450 mentok, titik y maks. 420
//         {startPoinMerah, point(265, 80), point(265, 150), point(265, 420), point(265, 420), point(265, 420)}, // 450 mentok, titik y maks. 420
//         {startPoinMerah, point(200, 300), point(265, 350), point(265, 420), point(265, 420), point(265, 420)},
//         {startPoinMerah, point(200, 300), point(265, 350), point(265, 420), point(265, 420), point(265, 420)},
//         {startPoinMerah, point(200, 300), point(265, 350), point(265, 420), point(265, 420), point(265, 420)},
//         {startPoinMerah, point(200, 300), point(265, 350), point(265, 420), point(265, 420), point(265, 420)},
//     }};

// posisi robot corner kiri

// vector<vector<vector<Point2i>>> jalanBiru = {
//     {
//         {startPoinBiru, point(600, 200), point(535, 200), point(555, 420), point(555, 420), point(555, 420)},
//         {startPoinBiru, point(600, 200), point(535, 220), point(555, 420), point(555, 420), point(555, 420)},
//         {startPoinBiru, point(600, 200), point(535, 220), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(650, 80), point(700, 300), point(560, 420), point(560, 420), point(560, 420)},
//         {startPoinBiru, point(600, 200), point(535, 200), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(600, 200), point(535, 200), point(535, 420), point(535, 420), point(535, 420)},
//     },
//     {
//         {startPoinBiru, point(600, 300), point(535, 350), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(600, 300), point(535, 350), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(600, 300), point(535, 350), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(535, 80), point(535, 150), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(535, 80), point(535, 150), point(535, 420), point(535, 420), point(535, 420)},
//         {startPoinBiru, point(600, 300), point(535, 350), point(535, 420), point(535, 420), point(535, 420)},

//     }};

// vector<vector<vector<Point2i>>> jalanMerah = {
//     {
//         {startPoinMerah, startPoinBolaKiri, point(250, 265), point(250, 265), point(250, 265), point(250, 265)},
//         {startPoinMerah, startPoinBolaKiri, point(150, 510), point(150, 510), point(150, 510), point(150, 510)},
//         {startPoinMerah, startPoinBolaKiri, point(250, 150), point(250, 150), point(250, 150), point(250, 150)},
//         {startPoinMerah, startPoinBolaKiri, point(250, 150), point(250, 150), point(250, 150), point(250, 150)},
//         {startPoinMerah, startPoinBolaKiri, point(250, 200), point(250, 200), point(250, 200), point(250, 200)},
//         {startPoinMerah, startPoinBolaKiri, point(250, 200), point(250, 200), point(250, 200), point(250, 200)},
//     },
//     {
//         {startPoinMerah, startPoinBolaKiri, point(220, 400), point(220, 400), point(220, 400), point(220, 400)},
//         {startPoinMerah, startPoinBolaKiri, point(220, 400), point(220, 400), point(220, 400), point(220, 400)},
//         {startPoinMerah, startPoinBolaKiri, point(150, 500), point(150, 500), point(150, 500), point(150, 500)},
//         {startPoinMerah, startPoinBolaKiri, point(220, 300), point(220, 300), point(220, 300), point(220, 300)},
//         {startPoinMerah, startPoinBolaKiri, point(150, 300), point(150, 300), point(150, 300), point(150, 300)},
//         {startPoinMerah, startPoinBolaKiri, point(200, 400), point(200, 400), point(200, 400), point(200, 400)},
//     }};

// posisi robot moda 5

// vector<vector<vector<Point2i>>> jalanBiru = {
//     {
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//     },
//     {
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, point(535, 165), point(535, 165)},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//         {startPoinBiru, startPoinBola, startPoinBiru},
//     }};

// vector<vector<vector<Point2i>>> jalanMerah = {
//     {
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//     },
//     {
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, startPoinBola, startPoinMerah},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//         {startPoinMerah, point(265, 165), point(265, 165)},
//     }};

// goal kanan

// void strategi()
// {
//     for (int i = 0; i < jalanBiru[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanBiru[tengah][samping][i], jalanBiru[tengah][samping][i + 1], Scalar((i + 1) * 50, 0, 0), 2);
//     }

//     for (int i = 0; i < jalanMerah[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanMerah[tengah][samping][i], jalanMerah[tengah][samping][i + 1], Scalar(0, 0, (i + 1) * 50), 2);
//     }

//     for (int i = 1; i < jalanMerah[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanMerah[tengah][samping][i], jalanBiru[tengah][samping][i], Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);

//         if (i == jalanMerah[tengah][samping].size() - 2)
//         {
//             switch (gawang){
//                 case 0:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(350, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 1:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(350, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 2:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(400, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//             }
//         }
//     }
// }

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

// goal moda 5

// void strategi()
// {
//     for (int i = 0; i < jalanBiru[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanBiru[tengah][samping][i], jalanBiru[tengah][samping][i + 1], Scalar((i + 1) * 50, 0, 0), 2);
//     }

//     for (int i = 0; i < jalanMerah[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanMerah[tengah][samping][i], jalanMerah[tengah][samping][i + 1], Scalar(0, 0, (i + 1) * 50), 2);
//     }

//     for (int i = 1; i < jalanMerah[tengah][samping].size() - 1; i++)
//     {
//         line(img, jalanMerah[tengah][samping][i], jalanBiru[tengah][samping][i], Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);

//         if (i == jalanMerah[tengah][samping].size() - 2)
//         {
//             switch (samping)
//             {
//             case 0:
//             case 1:
//             case 2:
//                 switch (gawang)
//                 {
//                 case 0:
//                     line(img, jalanBiru[tengah][samping][i + 1], point(450, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 1:
//                     line(img, jalanBiru[tengah][samping][i + 1], point(400, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 2:
//                     line(img, jalanBiru[tengah][samping][i + 1], point(400, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 }
//                 break;
//             case 3:
//             case 4:
//             case 5:
//                 switch (gawang)
//                 {
//                 case 0:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(350, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 1:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(350, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 case 2:
//                     line(img, jalanMerah[tengah][samping][i + 1], point(400, 600), Scalar((i + 1) * 50, (i + 1) * 50, (i + 1) * 50), 2);
//                     break;
//                 }
//                 break;
//             }
//         }
//     }
// }

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
