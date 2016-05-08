int duty = 0;
int steps = 64;
int sunrisespeed = 5000;
int i;
int j;
int pulsepin = 11;

int lookup[64] = {1,2,4,6,9,12,16,20,25,30,36,
42,49,56,64,72,81,90,100,110,121,132,
144,156,169,182,196,210,225,240,256,272,289,
306,324,342,361,380,400,420,441,462,484,506,
529,552,576,600,625,650,676,702,729,756,784,
812,841,870,900,930,961,992,992,992};


// there are freq steps in the phasing
void setup()
{
  pinMode(pulsepin, OUTPUT);
}

void loop()
{
  for (i=0; i<steps; i++)
  {
    duty = lookup[i] * 5;
    for (j=0; j<sunrisespeed; j++)
    {
      // one pulse of PWM
      digitalWrite(pulsepin, HIGH);
      delayMicroseconds(duty);
      digitalWrite(pulsepin, LOW);
      delayMicroseconds(5000-duty);
    }
  }
}
