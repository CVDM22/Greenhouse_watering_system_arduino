// includes
#include <math.h>
#include <LiquidCrystal.h>
#include <ezButton.h>
//#include <DateTime.h>

#define LED 13

//initialization of the LCD
LiquidCrystal lcd(46, 48, 39, 38, 37, 36);

// initialization of the buttons
ezButton ok_button(28);
ezButton up_button(26);
ezButton down_button(27);
ezButton emergency_stop(29); // this button will be used for emergency shut down of mottor


unsigned wattering_time = 0; // in minutes

int menu_case = 0;

unsigned in_action = 0; //either wattering (1) or testing (2) or off (0)

unsigned long in_action_start_time = 0;

int done_flag = 0;

// for some reason the function thinks straight away that I have pressed the ok button
// though this is not the case. Also, removing the ok case in this menu wont allow the 
// other buttons to funciton. why? what is the cause of it? Maybe I should add 10k resistors?
// But then why wont the other buttons do the same? Is it because they are on the sides? 
// This would imply that there is a residue of current flowing through the button. Also, I 
// should test in the main menu wether or not the held button registers as a button input.
// The way it should function is to track the current flow and on rise turn read as a press
// input and on fall as a release input. I need to use the button.loop if i want for it to work


/* 30/05/2022
 * I should add the led lights for each button, this could be usefull to indicate when the button is pressed 
 * Moreover I should pass quality testing with long term use:
 * - 30 min done
 * - 1h done
 * - 1h:30min done
 * - 2h
 * - 5h
 * - 1d
 * - 2d
 * - 1 week (in field most prolly)
 */
void set_wattering_time()
{
  ok_button.loop();
  up_button.loop();
  down_button.loop();
  emergency_stop.loop();

  done_flag = 0;
  int i=0;
  

  lcd.clear();
  

  lcd.setCursor(0, 0);
  lcd.print("Laikas");
  lcd.setCursor(0, 1);
  //lcd.print(wattering_time);
  lcd.print(wattering_time);
  delay(300);


  
  while (done_flag == 0)
  {
    ok_button.loop();
    up_button.loop();
    down_button.loop();
    emergency_stop.loop();

  
    //lcd.setCursor(0, 1);
    //lcd.print(i);
    
    //delay(300);
    if (up_button.isPressed())
    {
      wattering_time++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Laikas");
      lcd.setCursor(0, 1);
      lcd.print(wattering_time);
      //delay(300);
    }
    if (down_button.isPressed())
    {
      wattering_time--;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Laikas");
      lcd.setCursor(0, 1);
      lcd.print(wattering_time);
      //delay(300);
    }
    if (ok_button.isPressed())
    {
      done_flag = 1;
      //lcd.clear();
      //lcd.setCursor(0,1);
      //delay(300);
    }
    i++;

    //lcd.setCursor(0, 1);
  //lcd.print(wattering_time);
  //lcd.print(i);
  //delay(3000);
  }
  
  
}

void main_menu_display()
{
  
  if (menu_case == 0)
  {
    menu_case_0_display();
  }

  if (menu_case == 1)
  {
    menu_case_1_display();
  }

  if (menu_case == 2)
  {
    menu_case_2_display();
  }

}

// menu display with regards to the case

void menu_case_0_display()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
  lcd.print("LIEJ TRUK"); //Create a scrolling function 
}

void menu_case_1_display()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
  lcd.print("LIEJ"); //Create a scrolling function 
}

void menu_case_2_display()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
  lcd.print("TEST");
}

// change the menu case variable and increment it correctly 

void increment_menu_case()
{
  if (menu_case > 1)
  {
    menu_case = 0;
  }
  else
  {
    menu_case++;
  }
}


void decrement_menu_case()
{
  if (menu_case < 1)
  {
    menu_case = 2;
  }
  else
  {
    menu_case--;
  }
}

// display testing menu function 

void display_testing()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTING");
  lcd.setCursor(0, 1);
  lcd.print("TESTING");
}

// function to shut down the valve, this shouldnt be used for the emergency shut down as it is not as safe, better 
// to do the emergency shut down, the valve's + pole should be connected to the ground (via a resistor) thus breaking
// the current flow. for the moment this should handle the led light only for the testing purposes.

void shutdown_valve()
{
  // add the code
}
//

void wattering_testing(unsigned long t_start)
{
  ok_button.loop();
  up_button.loop();
  down_button.loop();
  emergency_stop.loop();
  int interrupt_request = 0;

  while (((millis() - t_start) < 6*1000UL) && (interrupt_request == 0))
  {
    ok_button.loop();
    up_button.loop();
    down_button.loop();
    emergency_stop.loop();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TESTING, liko min:");
    lcd.setCursor(0, 1);
    lcd.print(ceil((t_start + 6*1000UL - millis())/(1000UL)));  //prints out the mins left
    Serial.println("inside while");
    //OK interrupt
    if (ok_button.isPressed())
    {
      interrupt_request = 1; //flag to leave the while loop 
      Serial.println("inside if");
    }
    
  }
  // shut down the valve 
  
  main_menu_display();
}

// .loop is necessary to reset the button variable
void wattering(unsigned long t_start, unsigned long t_end)
{
  ok_button.loop();
  up_button.loop();
  down_button.loop();
  emergency_stop.loop();
  int interrupt_request = 0;

  while (((millis() - t_start) < t_end*1000UL) && (interrupt_request == 0))
  {
    ok_button.loop();
    up_button.loop();
    down_button.loop();
    emergency_stop.loop();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LIEJA, liko min:");
    lcd.setCursor(0, 1);
    lcd.print(ceil((t_start + t_end*1000UL - millis())/(1000UL)));  //prints out the mins left
    Serial.println("inside while");
    //OK interrupt
    if (ok_button.isPressed())
    {
      interrupt_request = 1; //flag to leave the while loop 
      Serial.println("inside if");
    }
    Serial.println(interrupt_request);
  }
  // shut down the valve 
  
  main_menu_display();
}

// valve shutdown manager depending on the cases

void shutdown_valve_testing(unsigned t_start)
{
  if ((millis() - t_start) > 60*1000L) //
  {
    //shut down valve 
    main_menu_display();
    in_action = 0;
  }
}

void shutdown_valve_wattering(unsigned t_start, unsigned t_end)
{
  if ((millis() - t_start) > t_end)
  {
    //shut down valve
  }
}

long minutes2mili(unsigned mins)
{
  return mins*60*1000L;
}


void setup() 
{
  Serial.begin(9600);

  pinMode(LED, OUTPUT); //led
  
  // button setup
  ok_button.setDebounceTime(50);
  up_button.setDebounceTime(50);
  down_button.setDebounceTime(50);
  emergency_stop.setDebounceTime(50);
  
  // put your setup code here, to run once:
  // lcd setup and starting state
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Labas!");
  delay(1000);

  main_menu_display();

}

void loop() 
{
  ok_button.loop();
  up_button.loop();
  down_button.loop();
  emergency_stop.loop();
  Serial.print("TIME SINCE START:");
  Serial.println(millis());
  Serial.println("");
  Serial.print("WATTERING TIME:");
  Serial.println(wattering_time);
  Serial.println("");

  // put your main code here, to run repeatedly:
  // in action is prolly unnecessary here, this is handled with the IRQs
  // ill have to check it out this evening
  if ((emergency_stop.isPressed()))
  {
    //shut down valve
  }

  if ((up_button.isPressed()))
  {
    increment_menu_case();
    main_menu_display();
  }

  if ((down_button.isPressed()))
  {
    decrement_menu_case();
    main_menu_display();
  }


  if ((ok_button.isPressed()))
  {
    if (menu_case == 0)
    {
      delay(500);
      set_wattering_time();
      main_menu_display();
    }
    if (menu_case == 1)
    {
      digitalWrite(LED, HIGH);
      in_action_start_time = millis(); //get the time when the valve was turned on
      wattering(in_action_start_time, wattering_time);
      digitalWrite(LED, LOW);
    }
    if (menu_case == 2)
    {
      digitalWrite(LED, HIGH);
      in_action_start_time = millis(); //get the time when the valve was turned on
      wattering_testing(in_action_start_time);
      digitalWrite(LED, LOW);
      /*
      display_testing();
      // open valve
      in_action=2;  //set the valve on flag to 2
      in_action_start_time = millis(); //get the time when the valve was turned on
      */
    }
    
  // check for the in action flag and shut it off after the time has ended
  
  // testing if this will brake something, in case itll break, remove the comments bellow (26/05/2022)
/*
  if (in_action > 0)
  {
    if (in_action == 2)
    {
       shutdown_valve_testing(in_action_start_time);
       
    }
    if (in_action == 1)
    {
       shutdown_valve_wattering(in_action_start_time, minutes2mili(wattering_time));
       in_action = 0;
    }
    else //in case there is a bug
    {
      // shut down the watter
      in_action = 0;
    }
  }
 */   
  }
  Serial.print("CASE: ");
  Serial.println(menu_case);
  Serial.println(" ");

  //Serial.println(in_action);
  //Serial.println(millis() - in_action_start_time);

//  if (in_action == 0)
//  {
//    main_menu_display();
    
//  }
  

}
