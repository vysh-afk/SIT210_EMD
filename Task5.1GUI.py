import tkinter as tk
import RPi.GPIO as GPIO

# GPIO setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Pin configuration for LEDs
RED_LED = 17
GREEN_LED = 27
BLUE_LED = 22

# Setting up GPIO pins as output
GPIO.setup(RED_LED, GPIO.OUT)
GPIO.setup(GREEN_LED, GPIO.OUT)
GPIO.setup(BLUE_LED, GPIO.OUT)

# Function to turn off all LEDs
def turn_off_all():
    GPIO.output(RED_LED, GPIO.LOW)
    GPIO.output(GREEN_LED, GPIO.LOW)
    GPIO.output(BLUE_LED, GPIO.LOW)

# Function to turn on the corresponding LED
def turn_on_led(led_color):
    # Check if the selected LED is already on, if so turn it off
    if led_color == 'Red':
        if GPIO.input(RED_LED) == GPIO.HIGH:
            GPIO.output(RED_LED, GPIO.LOW)  # Turn off Red LED
        else:
            turn_off_all()  # Turn off all LEDs first
            GPIO.output(RED_LED, GPIO.HIGH)  # Turn on Red LED
    elif led_color == 'Green':
        if GPIO.input(GREEN_LED) == GPIO.HIGH:
            GPIO.output(GREEN_LED, GPIO.LOW)  # Turn off Green LED
        else:
            turn_off_all()  # Turn off all LEDs first
            GPIO.output(GREEN_LED, GPIO.HIGH)  # Turn on Green LED
    elif led_color == 'Blue':
        if GPIO.input(BLUE_LED) == GPIO.HIGH:
            GPIO.output(BLUE_LED, GPIO.LOW)  # Turn off Blue LED
        else:
            turn_off_all()  # Turn off all LEDs first
            GPIO.output(BLUE_LED, GPIO.HIGH)  # Turn on Blue LED

# Function to handle exit
def exit_program():
    turn_off_all()
    GPIO.cleanup()
    root.quit()

# Creating the main window
root = tk.Tk()
root.title("LED Control")

# Variable to hold selected LED color
selected_led = tk.StringVar()
selected_led.set(None)  # No selection by default

# Radio buttons for LED control
tk.Radiobutton(root, text="Red LED", variable=selected_led, value="Red", command=lambda: turn_on_led('Red')).pack()
tk.Radiobutton(root, text="Green LED", variable=selected_led, value="Green", command=lambda: turn_on_led('Green')).pack()
tk.Radiobutton(root, text="Blue LED", variable=selected_led, value="Blue", command=lambda: turn_on_led('Blue')).pack()

# Exit button
exit_button = tk.Button(root, text="Exit", command=exit_program)
exit_button.pack()

# Run the main loop
root.mainloop()

# Cleanup the GPIO pins on exit
GPIO.cleanup()
