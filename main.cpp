//
// Created by 張暐翔 on 2022/12
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

struct dayPrice
{
    double day;
    double price;
};
/// Filter Function
void kFilter(double value , bool* kArray , double* kValue, int);
void dFilter(double threshold , bool* dArray , double* dValue, int);
void macdFilter(double threshold , bool* macdArray , double* macdValue, int );
void boolingerFilter(double threshold , bool* boolingerArray , double* boolingerValue, int );
void MoMFilter(double threshold , bool* MoMArray , double* MoMValue, int );
void MA5Filter(double threshold , bool* MA5Array , double* MA5Value, int);
void MA10Filter(double threshold , bool* MA10Array , double* MA10Value, int);
void MA20Filter(double threshold , bool* MA20Array , double* MA20Value, int);
void RSIFilter(double threshold , bool* RSIArray , double* RSIValue, int);

dayPrice* generateDay(bool selected[20], double threshold[20],  double ** aDoubleArray);



double K(double** sheet, double date); //找當日k值（應界在0到100之間）
double D(double** sheet, double date); //找當日d值（應界在0到100之間）
double MACD(double** sheet, double date); //找當日MACD值
bool inBoolingerBand(double** sheet, double date, int days, int times); //當日股票價格是否落在布林通道, 使用者可輸入days來決定要用幾天的平均線、輸入times來決定要用幾倍標準差。
double boolingerUpper(double** sheet, double date, int days, int times); //當日布林通道上界
double boolingerLower(double** sheet, double date, int days, int times); //當日布林通道下界
double MoM(double** sheet, double date); //找當天往前推30天對30到60天的營收成長（應屆在-1到1之間）
double MA5(double** sheet, double date); //找當日5MA線
double MA10(double** sheet, double date); //找當日10MA線
double MA20(double** sheet, double date); //找當日20MA線
double RSI(double** sheet, double date); //找6日RSI值（應界在0到100之間）
///////////////指標函式結束//////////////////

///////////////KD值函式///////////////////
double RSV(double** sheet, int dateIndex); //找當日rsv值
double findHighest(double** sheet, int dateIndex); //找當日往回算9天內最高價
double findLowest(double** sheet, int dateIndex); //找當日往回算9天內最低價
double kRecursive(double** sheet, int dateIndex);
double dRecursive(double** sheet, int dateIndex);
//////////////KD值函式結束//////////////////

//////////////MACD值函式///////////////////
double demandIndex(double** sheet, int dateIndex); //找當日demand index
double EMA12(double** sheet, int dateIndex); //找當日EMA12
double EMA26(double** sheet, int dateIndex); //找當日EMA26
double DIF(double** sheet, int dateIndex); //找當日DIF
double MACDRecursive(double** sheet, int dateIndex);
/////////////MACD值函式結束/////////////////

////////////布林通道函式////////////////////
double findStandardDeviation(double** sheet, double date, int days); //找MA線標準差
double findAverage(double** sheet, double date, int days); //找days日平均
////////////布林通道函式結束////////////////

//////////////通用函式////////////////////
int findDateIndex(double** sheet, double date); //找符合日期的index
double findClose(double** sheet, int dateIndex); //找當日收盤價
double findHigh(double** sheet, int dateIndex); //找當日最高價
double findLow(double** sheet, int dateIndex); //找當日最低價
///////////////通用函式結束//////////////////
/////////////////End of Function Titles///////////////////////////

////////////////////Functions////////////////////
///////////////////指標函式//////////////////////
using namespace std;
//#include <SFML/Graphics.hpp>
void loadPrice(const string& fileName, string** priceInfo, const int colNum);
void loadEco();
double** toDouble(string** , int, int);
double** dataToCsv(double** doubleArr, const int arrRow, const int arrCol);

//#include <SFML/Graphics.hpp>


int main()
{

    //    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }


    string fileName = R"(/Users/markoo/Desktop/mydata.csv)";
    const int rowNum = 7857;
    const int colNum = 16;
    string** priceInfo = new string*[rowNum]; // 7900為最大列數
    for(int i = 0; i < rowNum; i++){
        priceInfo[i] = new string[colNum];
    }
    loadPrice(fileName, priceInfo, colNum);
    cout << priceInfo[1][4];

    double **aDoubleArray = toDouble(priceInfo, 16, 7857);
    for (int i = 1; i < 7857; i ++)
    {
        for (int j = 0; j < 5; j ++)
        {
            cout << aDoubleArray[i][j] << " ";
        }
        cout << endl;
    }
    bool selectSate[20] = {true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
    double threshold[20] = {30, 30, 30, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    dayPrice* dayPriceArray = generateDay(selectSate, threshold, aDoubleArray);
    cout << dayPriceArray[0].day << "," << dayPriceArray[0].price << endl;



    return 0;
}

void loadPrice(const string& fileName, string** priceInfo, const int colNum){ // 需要dateNum
    ifstream ifs;                            //创建流对象
    ifs.open(fileName, ios::in);    //打开文件
    if (!ifs.is_open())                        //判断文件是否打开
    {
        cout << "openWrong";
        return;
    }
    vector<string> item;        //用于存放文件中的一行数据
    string temp;                //临时存储文件中的一行数据
    while (getline(ifs, temp))  //利用 getline（）读取文件中的每一行，并放入到 item 中
    {
        item.push_back(temp);
    }
    int rowCount = 0;
    //遍历文件中的每一行数据
    //auto it = item.begin();
    for (auto it = item.begin() ; it != item.end() ; it++)
    {
        string str;
        //其作用是把字符串分解为单词(在此处就是把一行数据分为单个数据)
        istringstream istr(*it);
        // 一列一列輸入，第0個item是日期
        for (int i = 0; i < colNum; i++){
            getline(istr, str, ',');
            priceInfo[rowCount][i] = str;
//            if(rowCount == 0) cout << str;
            if (i < colNum - 1) cout << priceInfo[rowCount][i] << '\t';
            else cout << priceInfo[rowCount][i] << endl;
        }
        //it ++;
        rowCount++;
    }
}
void loadEco(char* fileName, double** ecoInfo, int ecoNum){
    ifstream myFile;
    myFile.open(fileName);
    myFile.close();
}

double** toDouble(string ** stringArray, int colNum, int rowNum){
    // 此函數需輸入 一個 二維的String陣列、行數以及列數，並吐出一個大小相同的 double二維陣列。
    double** outputArray = new double* [rowNum]; // 新增一個2維double Array，一格一格更新，最後return回去

    for (int i = 1 ; i < rowNum; i ++) // 每一列
    {
        outputArray[i] = new double [colNum];
        for (int j = 0; j < colNum; j ++) // 每一行
        {
            string aString = stringArray[i][j];
            if (j == 0) //  第一column，代表此string為日期
            {
                string yearStr = aString.substr(0,4);
                string monthStr = aString.substr(5,7);
                string dateStr = aString.substr(8);
                double date = 0;
                date = stod(yearStr) * 10000 + stod(monthStr) * 100 + stod(dateStr);
                outputArray[i][j] = date;
            }
            else // 代表此string為單純數字
            {
                double num = stod(aString); // 把數字轉為 double
                outputArray[i][j] = num;
            }
        }
    }
    return outputArray;
}
void writeCSV(double** doubleArr, const int arrRow, const int arrCol){
    //writefile
    fstream file;
    file.open("write.csv");
    for (int i=0;i<arrRow;i++)
    {
        for(int j = 0; j < arrCol; j++){
            if (j == arrCol - 1) file << doubleArr[i][j] << endl;
            else file << doubleArr[i][j] << ",";
        }

    }
    file.close();
    return;
}
double K(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double k = kRecursive(sheet, i);
    return k;
}

double D(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double d = dRecursive(sheet, i);
    return d;
}

double MACD(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double MACD = MACDRecursive(sheet, i);
    return MACD;
}

bool inBoolingerBand(double** sheet, double date, int days, int times){
    int dateIndex = findDateIndex(sheet, date);
    double priceToday = findClose(sheet, dateIndex);
    if(priceToday < boolingerUpper(sheet, date, days, times) and priceToday > boolingerLower(sheet, date, days, times)){
        return true;
    }
    else{
        return false;
    }
}

double boolingerUpper(double** sheet, double date, int days, int times){
    double standardDeviation = findStandardDeviation(sheet, date, days);
    double average = findAverage(sheet, date, days);
    double upperBound = average + times * standardDeviation;
    return upperBound;
}

double boolingerLower(double** sheet, double date, int days, int times){
    double standardDeviation = findStandardDeviation(sheet, date, days);
    double average = findAverage(sheet, date, days);
    double lowerBound = average - times * standardDeviation;
    return lowerBound;
}

double MoM(double** sheet, double date){
    double returnThisMonth = 0;
    double returnLastMonth = 0;

    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 30; i++){
        returnThisMonth += sheet[dateIndex + i][4] / 30;
    }
    for(int i = 30; i < 60; i++){
        returnLastMonth += sheet[dateIndex + i][4] / 30;
    }

    double MoM = static_cast<double>(static_cast<int>(((returnThisMonth - returnLastMonth) / returnLastMonth) * 100)) / 100;
    return MoM;
}

double MA5(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 5; i++){
        MA += sheet[dateIndex + i][4] / 5;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double MA10(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 10; i++){
        MA += sheet[dateIndex + i][4] / 10;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double MA20(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 20; i++){
        MA += sheet[dateIndex + i][4] / 20;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double RSI(double** sheet, double date){
    int dateIndex = findDateIndex(sheet, date);
    double increase = 0;
    double decrease = 0;
    for(int i = 0; i < 6; i++){
        if(sheet[dateIndex + i][4] - sheet[dateIndex + i + 1][4] >= 0){
            increase += sheet[dateIndex + i][4] - sheet[dateIndex + i + 1][4];
        }
        else{
            decrease += sheet[dateIndex + i + 1][4] - sheet[dateIndex + i][4];
        }
    }
    double rsi = static_cast<double>(static_cast<int>((increase / (increase + decrease)) * 10000)) / 100;
    return rsi;
}
//////////////////指標函式結束//////////////////

///////////////////KD值函式////////////////////
double RSV(double** sheet, int dateIndex){
    double close = findClose(sheet, dateIndex);
    double high = findHighest(sheet, dateIndex);
    double low = findLowest(sheet, dateIndex);
    double rsv = static_cast<double>(static_cast<int>(((close - low) / (high - low)) * 10000)) / 100;
    return rsv;
}

double findHighest(double** sheet, int dateIndex){
    double highest = 0;
    for(int j = 0; j < 9; j++){
        if (dateIndex + j == 7888)
            break;
        if(sheet[dateIndex + j][2] > highest){
            highest = sheet[dateIndex + j][2];
        }
    }
    return highest;
}

double findLowest(double** sheet, int dateIndex){
    double lowest = 100000;
    for(int j = 0; j < 9; j++){
        if (dateIndex + j == 7888)
            break;
        if(sheet[dateIndex + j][3] < lowest){
            lowest = sheet[dateIndex + j][3];
        }
    }
    return lowest;
}

double kRecursive(double** sheet, int dateIndex){
    if(dateIndex == 7888){
        return 50;
    }
    else{
        //cout << dateIndex << endl;
        double kYesterday = kRecursive(sheet, dateIndex + 1);
        double rsvToday = RSV(sheet, dateIndex);
        double kToday = static_cast<double>(static_cast<int>((kYesterday * (2 / 3) + rsvToday / 3) * 100)) / 100;
        return kToday;
    }
}

double dRecursive(double** sheet, int dateIndex){
    if(dateIndex == 7888){
        return 50;
    }
    else{
        double dYesterday = dRecursive(sheet, dateIndex + 1);
        double kToday = kRecursive(sheet, dateIndex);
        double dToday = static_cast<double>(static_cast<int>((dYesterday * (2 / 3) + kToday / 3) * 100)) / 100;
        return dToday;
    }
}
///////////////////KD值函式結束//////////////////

///////////////////MACD值函式////////////////////
double demandIndex(double** sheet, int dateIndex){
    double DI = static_cast<double>(static_cast<int>(((findHigh(sheet, dateIndex) + findLow(sheet, dateIndex) + 2 * findClose(sheet, dateIndex)) / 4) * 10000)) / 100;
    return DI;
}

double EMA12(double** sheet, int dateIndex){
    if(dateIndex == 7878){
        double DI12 = 0;
        for(int i = 0; i < 12; i++){
            DI12 += static_cast<double>(static_cast<int>((demandIndex(sheet, dateIndex + i) / 12) * 100)) / 100;
        }
        return DI12;
    }
    else{
        double EMA12Yesterday = EMA12(sheet, dateIndex + 1);
        double DIToday = demandIndex(sheet, dateIndex);
        double EMA12Today = static_cast<double>(static_cast<int>((EMA12Yesterday * (11 / 13) + DIToday * (2 / 13)) * 100)) / 100;
        return EMA12Today;
    }
}

double EMA26(double** sheet, int dateIndex){
    if(dateIndex == 7864){
        double DI26 = 0;
        for(int i = 0; i < 26; i++){
            DI26 += static_cast<double>(static_cast<int>((demandIndex(sheet, dateIndex + i) / 26) * 100)) / 100;
        }
        return DI26;
    }
    else{
        double EMA26Yesterday = EMA26(sheet, dateIndex + 1);
        double DIToday = demandIndex(sheet, dateIndex);
        double EMA26Today = static_cast<double>(static_cast<int>((EMA26Yesterday * (25 / 27) + DIToday * (2 / 27)) * 100)) / 100;
        return EMA26Today;
    }
}

double DIF(double** sheet, int dateIndex){
    double ema12 = EMA12(sheet, dateIndex);
    double ema26 = EMA26(sheet, dateIndex);
    double DIF = ema12 - ema26;
    return DIF;
}

double MACDRecursive(double** sheet, int dateIndex){
    if(dateIndex > 7864)
    {
        return 50;
    }
    else
    {
        cout << dateIndex << endl;
        double MACDYesterday = MACDRecursive(sheet, dateIndex + 1);
        double DIFToday = DIF(sheet, dateIndex);
        double MACDToday = static_cast<double>(static_cast<int>((MACDYesterday * (8 / 10) + DIFToday * (2 / 10)) * 100)) / 100;
        return MACDToday;
    }
}
//////////////////MACD值函式結束///////////////////

/////////////////布林通道函式//////////////////////
double findAverage(double** sheet, double date, int days){
    int dateIndex = findDateIndex(sheet, date);
    double average = 0;
    for(int i = 0; i < days; i++){
        average += sheet[dateIndex + i][4] / days;
    }
    return average;
}
double findStandardDeviation(double** sheet, double date, int days){
    //this function finds standard deviation by the equation E(pow(x, 2)) - pow((E(x)), 2).
    int dateIndex = findDateIndex(sheet, date);

    //E(pow(x, 2))
    double e2 = 0;
    for(int i = 0; i < days; i++){
        e2 += pow(sheet[dateIndex + i][4], 2);
    }
    e2 = e2 / days;
    //pow(E(x), 2)
    double e = findAverage(sheet, date, days);
    e = pow(e, 2);

    double variation = e2 - e;
    double standardDeviation = pow(variation, 1 / 2);
    standardDeviation = static_cast<double>(static_cast<int>((standardDeviation) * 100)) / 100;
    return standardDeviation;
}
//////////////////布林通道函式結束//////////////////

//////////////////通用函式////////////////////////
int findDateIndex(double** sheet, double date){
    int dateIndex = 0;
    for(int i = 1; i < 7888; i++){
        if(sheet[i][0] == date){
            dateIndex = i;
            break;
        }
    }
    return dateIndex;
}

double findClose(double** sheet, int dateIndex){
    //cout << sheet[dateIndex][1] << sheet[dateIndex][2] << sheet[dateIndex][3];
    //double close = sheet[dateIndex][4];
    //return close;
    //cout << sheet[dateIndex][4] << endl;
    return sheet[dateIndex][4];
}

double findHigh(double** sheet, int dateIndex){
    double high = sheet[dateIndex][2];
    return high;
}

double findLow(double** sheet, int dateIndex){
    double low = sheet[dateIndex][3];
    return low;
}
///////////////////通用函式結束/////////////////////
void kFilter(double threshold  , bool* kArray , double* kValue , int rowNum)
{
    for (int i = 1 ; i < rowNum ; i++)
    {
        if (kValue[i] >= threshold )
            kArray[i] = true;
    }
}
void dFilter(double threshold , bool* dArray , double* dValue, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (dValue[i] >= threshold)
            dArray[i] = true;
    }
}
void macdFilter(double threshold , bool* macdArray , double* macdValue, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (macdValue[i] >= threshold)
            macdArray[i] = true;
    }
}
void boolingerFilter(double threshold , bool* boolingerArray , double* boolingerValue, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (boolingerValue[i] >= threshold)
            boolingerArray[i] = true;
    }
}
void MoMFilter(double threshold , bool* MoMArray , double* MoMValue, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (MoMValue[i] >= threshold)
            MoMArray[i] = true;
    }
}
void MA5Filter(double threshold , bool* MA5Array , double* MA5Value, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (MA5Value[i] >= threshold)
            MA5Array[i] = true;
    }
}
void MA10Filter(double threshold , bool* MA10Array , double* MA10Value, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (MA10Value[i] >= threshold)
            MA10Array[i] = true;
    }
}
void MA20Filter(double threshold , bool* MA20Array , double* MA20Value, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (MA20Value[i] >= threshold)
            MA20Array[i] = true;
    }
}
void RSIFilter(double threshold , bool* RSIArray , double* RSIValue, int rowNUm)
{
    for (int i = 1 ; i < rowNUm; i++)
    {
        if (RSIValue[i] >= threshold)
            RSIArray[i] = true;
    }
}
/////////////////////End of Functions////////////////////////////
dayPrice* generateDay(bool selected[20], double threshold[20],  double ** aDoubleArray)
{
    int condition = 0;
    for (int i = 0; i < 20; i ++)
    {
        if (selected[i] != 0)
        {
            condition ++;
        }
    }
    double* kValue = new double [7857];
    double* dValue = new double [7857];
    double* macdValue = new double [7857];
    double* boolingerUpperValue = new double [7857];
    double* boolingerLowerValue = new double [7857];
    double* MoMValue = new double [7857];
    double* MA5Value = new double [7857];
    double* MA10Value = new double [7857];
    double* MA20Value = new double [7857];
    double* RSIValue = new double [7857];

    bool* kArray = new bool [7857];
    bool* dArray = new bool [7857];
    bool* macdArray = new bool [7857];
    bool* boolingerUpperArray = new bool [7857];
    bool* boolingerLowerArray = new bool [7857];
    bool* MoMArray = new bool [7857];
    bool* MA5Array = new bool [7857];
    bool* MA10Array = new bool [7857];
    bool* MA20Array = new bool [7857];
    bool* RSIArray = new bool [7857];

    bool * goodDay = new bool [7857];
    for (int i = 0; i < 7857; i ++)
    {
        goodDay[i] = false;
    }

    for (int i = 1 ; i < 7857 ; i++)
    {
        kArray[i] = false;
        dArray[i] = false;
        macdArray[i] = false;
        boolingerLowerArray[i] = false;
        boolingerLowerArray[i] = false;
        MoMArray[i] = false;
        MA5Array[i] = false;
        MA10Array[i] = false;
        MA20Array[i] = false;
        RSIArray[i] = false;
    }

    for (int i = 1 ; i < 7857 ; i++)
    {
        kValue[i] = aDoubleArray[i][14];
        dValue[i] = aDoubleArray[i][15];
        macdValue[i] = aDoubleArray[i][11];
        boolingerUpperValue[i] = aDoubleArray[i][8];
        boolingerLowerValue[i] = aDoubleArray[i][10];
        MoMValue[i] = aDoubleArray[i][13];
        MA5Value[i] = aDoubleArray[i][5];
        MA10Value[i] = aDoubleArray[i][6];
        MA20Value[i] = aDoubleArray[i][7];
        RSIValue[i] = aDoubleArray[i][12];
    }

    if (selected[0] == true)
        kFilter(threshold[0], kArray, kValue, 7857);
    if (selected[1] == true)
        dFilter(threshold[1], dArray, dValue, 7857);
    if (selected[2] == true)
        macdFilter(threshold[2], macdArray, macdValue, 7857);
    if (selected[3] == true)
        boolingerFilter(threshold[3], boolingerUpperArray, boolingerUpperValue, 7857);
    if (selected[4] == true)
        MoMFilter(threshold[4], boolingerLowerArray, boolingerLowerValue, 7857);
    if (selected[5] == true)
        MA5Filter(threshold[5], MA5Array, MA5Value, 7857);
    if (selected[6] == true)
        MA10Filter(threshold[6], MA10Array, MA10Value, 7857);
    if (selected[7] == true)
        MA20Filter(threshold[7], MA20Array, MA20Value, 7857);
    if (selected[8] == true)
        RSIFilter(threshold[8], RSIArray, RSIValue, 7857);
//    if (selected[9] == true)
//        macdFilter();
//    if (selected[10] == true)
//        kFilter();
//    if (selected[11] == true)
//        dFilter();
//    if (selected[12] == true)
//        macdFilter();
//    if (selected[13] == true)
//        kFilter();
//    if (selected[14] == true)
//        dFilter();
//    if (selected[15] == true)
//        macdFilter();
//    if (selected[16] == true)
//        kFilter();
//    if (selected[17] == true)
//        dFilter();
//    if (selected[18] == true)
//        macdFilter();
//    if (selected[19] == true)
//        dFilter();
    int goodDayCount  = 0;
    for (int i = 1; i < 7857; i ++)
    {
        if (kArray[i] + dArray[i] + macdArray[i] + boolingerUpperArray[i] + boolingerLowerArray[i] + MoMArray[i] + MA5Array[i] + MA10Array[i] + MA20Array[i] + RSIArray[i] == condition)
        {
            goodDay[i] = true;
            goodDayCount ++;
        }

    }
    int dayCount = 0;
    dayPrice * dayPriceArray = new dayPrice [goodDayCount];
    for (int i = 1; i < 7857; i ++)
    {
        if (goodDay[i] == true)
        {
            dayPriceArray[dayCount].day = aDoubleArray[i][0];
            dayPriceArray[dayCount].price = aDoubleArray[i][4];
        }
    }
    return dayPriceArray;
}



