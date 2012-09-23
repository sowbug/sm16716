/*
 * An Arduino controller for LED strings based on the SM16716 controller.
 *
 * By Mike Tsao (github.com/sowbug). Copyright 2012, all rights reserved.
 *
 * You might see these being sold as WS2801 strands, and you're wondering why
 * your off-the-shelf WS2801 code isn't working. This sketch will restore
 * your confidence that your lights aren't broken.
 *
 * This code isn't the fastest, but it is just about the simplest you'll
 * find. For performance, investigate code that uses hardware SPI.
 *
 * Beware that everyone seems to use different wiring color schemes. I
 * found the order of the wires is more reliable. Looking at the IC side
 * of the board, you'll find from left to right: V+, Data, Clock, Ground.
 */

#define DATA_PIN (11)
#define CLOCK_PIN (13)

#define LIGHT_COUNT (49)

uint32_t pixels[LIGHT_COUNT];

#define pinModeFast(x, y) pinMode(x, y)
#define digitalWriteFast(x, y) digitalWrite(x, y)

void setup() {
  pinModeFast(DATA_PIN, OUTPUT);
  pinModeFast(CLOCK_PIN, OUTPUT);
  digitalWriteFast(DATA_PIN, LOW);
  digitalWriteFast(CLOCK_PIN, LOW);
}

static void set_pixel(uint8_t index, uint32_t color) {
  pixels[index] = color & 0x00FFFFFF;
}

static void set_pixel_rgb(uint8_t index, uint32_t r, uint32_t g, uint32_t b) {
  set_pixel(index, r | (g << 8) | (b << 16));
}

static void toggle_clock() {
  digitalWriteFast(CLOCK_PIN, HIGH);
  digitalWriteFast(CLOCK_PIN, LOW);
}

static void write_pixel(uint8_t i) {
  const uint32_t MASK = ((uint32_t)(1) << 24);
  uint32_t p = pixels[i] | MASK;
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, (p & MASK) ? HIGH : LOW);
    toggle_clock();
    p <<= 1;
  }
}

static void write_blank_pixel() {
  int j = 25;
  while (j--) {
    digitalWriteFast(DATA_PIN, 0);
    toggle_clock();
  }
}

static void show() {
  digitalWriteFast(DATA_PIN, LOW);
  for (int i = 0; i < 50; i++) {
    toggle_clock();
  }
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    write_pixel(i);
  }
  write_blank_pixel();
  delay(1);
}

static void solid_color(uint32_t color) {
  for (int i = 0; i < LIGHT_COUNT; ++i) {
    set_pixel(i, color);
  }
}

static uint32_t simple_color(uint8_t color) {
  switch (color) {
  case 0:
    return 0x00ff0000;
  case 1:
    return 0x0000ff00;
  case 2:
    return 0x000000ff;
  case 3:
    return 0x00ffff00;
  case 4:
    return 0x00ff00ff;
  case 5:
    return 0x0000ffff;
  case 6:
    return 0x00ffffff;
  default:
    return 0x00330000;
  }
}

static uint32_t rand_color() {
  return simple_color(rand() % 7);
}

static uint32_t get_dimmer_mask(uint32_t value) {
  return value | (value << 8) | (value << 16);
}

void loop() {
  for (int i = 0; i < 7; ++i) {
    solid_color(simple_color(i));
    show();
    delay(1000);
  }

  uint32_t color = 0;
  for (int j = 0; j < 16; ++j) {
    uint32_t old_color = color;
    do {
      old_color = color;
      color = rand_color();
    } 
    while (old_color == color);
    for (int i = 0; i < LIGHT_COUNT; ++i) {
      set_pixel(i, color);
      show();
      delay(5);
    }
  }

  for (uint8_t k = 0; k < 7; ++k) {
    uint32_t color = simple_color(k);
    for (uint32_t j = 0; j < 255; ++j) {
      solid_color(color & get_dimmer_mask(j));
      show();
    }
    for (uint32_t j = 255; j != 0; --j) {
      solid_color(color & get_dimmer_mask(j));
      show();
    }
  }
}


