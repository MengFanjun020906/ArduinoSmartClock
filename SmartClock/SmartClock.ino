#include <dht11.h>   //引用dht11库文件，使得下面可以调用相关参数
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //引用I2C库
#include <DS1302.h>
#include<IRremote.h>

#define dht11Pin 8   //定义温湿度针脚号为8号引脚
dht11 dht;    //实例化一个对象
char buf1[50];
char buf2[50];
LiquidCrystal_I2C lcd(0x27,16,2); //这里是0x27
DS1302 rtc(14, 15, 16); //对应DS1302的RST,DAT,CLK
int RECV_PIN = 11;   //红外线接收器OUTPUT端接在pin 11
int Keynum=1;
IRrecv irrecv(RECV_PIN);   // 定义IRrecv 对象来接收红外线信号
decode_results results;   //解码结果放在decode_results构造的对象results里

void initRTCTime(void)//初始化RTC时钟
{
  rtc.writeProtect(false); //关闭写保护
  rtc.halt(false); //清除时钟停止标志
  Time t(2022, 12, 19, 16, 25, 50, 4); //新建时间对象 最后参数位星期数据，周日为1，周一为2以此类推
  rtc.time(t);//向DS1302设置时间数据
}
void printTime()//打印时间数据
{
  Time tim = rtc.time(); //从DS1302获取时间数据
  
  snprintf(buf1, sizeof(buf1), "%04d-%02d-%02d ",
           tim.yr, tim.mon, tim.date
           );
  snprintf(buf2, sizeof(buf2), "%02d:%02d:%02d",
           tim.hr, tim.min, tim.sec);

  Serial.println(buf1);
  Serial.println(buf2);
}
void setup()    //初始化函数，只执行一次
{
  Serial.begin(9600);      //设置波特率参数
  pinMode(dht11Pin, OUTPUT);    //通过定义将Arduino开发板上dht11Pin引脚(8号口)的工作模式转化为输出模式

  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮
  initRTCTime();
  irrecv.enableIRIn(); //    启动红外解码
  delay(2000);
}

void loop()     //loop函数，重复循环执行
{
  if (irrecv.decode(&results)!=0)  
  {        
      delay(500);
      Keynum++;
      delay(500);
      lcd.clear();
      if(Keynum>=3)
      {
        Keynum=1;
      }
      irrecv.resume();
  }
  switch(Keynum)
  {
    case 1:  SerialTem();ther();break;
    case 2:  printTime();
            Time tim = rtc.time(); //从DS1302获取时间数据
            lcd.setCursor(0,0);
            lcd.print(buf1);
            lcd.setCursor(0,1);
            lcd.print(buf2);
            break;
    default: break;
  }   
}
void ther()//温湿度计
{
  int tol = dht.read(dht11Pin);    //将读取到的值赋给tol
  int temp = (float)dht.temperature; //将温度值赋值给temp
  int humi = (float)dht.humidity; //将湿度值赋给humi

  lcd.setCursor(0,0);
  lcd.print("Tem:");
  lcd.setCursor(4,0);
  lcd.print(temp);
  lcd.setCursor(6,0);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("Hum:");
  lcd.setCursor(4,1);
  lcd.print(humi);
  lcd.setCursor(6,1);
  lcd.print("%");
}
void SerialTem()//串口打印温度湿度
{
  int tol = dht.read(dht11Pin);    //将读取到的值赋给tol
  int temp = (float)dht.temperature; //将温度值赋值给temp
  int humi = (float)dht.humidity; //将湿度值赋给humi
  Serial.print("Temperature:");     //在串口打印出Tempeature:
  Serial.print(temp);       //在串口打印温度结果
  Serial.println(".C");    //在串口打印出℃
  Serial.print("Humidity:");     //在串口打印出Humidity:
  Serial.print(humi);     //在串口打印出湿度结果
  Serial.println("%");     //在串口打印出%  
}
