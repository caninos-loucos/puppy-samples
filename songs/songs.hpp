#pragma once

#define NOTE_C4 (261)
#define NOTE_CS4 (277)
#define NOTE_D4 (293)
#define NOTE_DS4 (311)
#define NOTE_E4 (329)
#define NOTE_F4 (349)
#define NOTE_FS4 (369)
#define NOTE_G4 (392)
#define NOTE_GS4 (415)
#define NOTE_A4 (440)
#define NOTE_AS4 (466)
#define NOTE_B4 (493)

#define NOTE_C5 (523)
#define NOTE_CS5 (554)
#define NOTE_D5 (587)
#define NOTE_DS5 (622)
#define NOTE_E5 (659)
#define NOTE_F5 (698)
#define NOTE_FS5 (739)
#define NOTE_G5 (783)
#define NOTE_GS5 (830)
#define NOTE_A5 (880)
#define NOTE_AS5 (932)
#define NOTE_B5 (987)

#define NOTE_C6 (1046)
#define NOTE_CS6 (1108)
#define NOTE_D6 (1174)
#define NOTE_DS6 (1244)
#define NOTE_E6 (1318)
#define NOTE_F6 (1397)
#define NOTE_FS6 (1479)
#define NOTE_G6 (1567)
#define NOTE_GS6 (1661)
#define NOTE_A6 (1760)
#define NOTE_AS6 (1864)
#define NOTE_B6 (1975)

#define REST 0

class Song {
public:
  uint32_t bpm;
  std::vector<uint32_t> melody;
  std::vector<uint32_t> durations;
  Song(uint32_t bpm, std::vector<uint32_t> melody,
       std::vector<uint32_t> durations)
      : bpm(bpm), melody(melody), durations(durations) {}
};

static std::vector<uint32_t> melodyNokia = {
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5, NOTE_B4, NOTE_D4,
    NOTE_E4, NOTE_B4, NOTE_A4,  NOTE_CS4, NOTE_E4,  NOTE_A4, REST};

static std::vector<uint32_t> durationsNokia = {8, 8, 4, 4, 8, 8, 4,
                                               4, 8, 8, 4, 4, 8, 8};

static std::vector<uint32_t> melodyBirthday = {
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,  NOTE_C4,
    NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_C4,  NOTE_C4,

    NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4,
    NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4};

static std::vector<uint32_t> durationsBirthday = {
    4, 8, 4, 4, 4, 2, 4, 8, 4, 4, 4, 2, 4, 8,

    4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 2};

static std::vector<uint32_t> melodyStarWars = {
    NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_F5,  NOTE_C6, NOTE_AS5, NOTE_A5,
    NOTE_G5,  NOTE_F6,  NOTE_C6,  NOTE_AS5, NOTE_A5, NOTE_G5,  NOTE_F6,
    NOTE_C6,  NOTE_AS5, NOTE_A5,  NOTE_AS5, NOTE_G5, NOTE_C5,  NOTE_C5,
    NOTE_C5,  NOTE_F5,  NOTE_C6,  NOTE_AS5, NOTE_A5, NOTE_G5,  NOTE_F6,
    NOTE_C6,

    NOTE_AS5, NOTE_A5,  NOTE_G5,  NOTE_F6,  NOTE_C6, NOTE_AS5, NOTE_A5,
    NOTE_AS5, NOTE_G5,  NOTE_C5,  NOTE_C5,  NOTE_D5, NOTE_D5,  NOTE_AS5,
    NOTE_A5,  NOTE_G5,  NOTE_F5,  NOTE_F5,  NOTE_G5, NOTE_A5,  NOTE_G5,
    NOTE_D5,  NOTE_E5,  NOTE_C5,  NOTE_C5,  NOTE_D5, NOTE_D5,  NOTE_AS5,
    NOTE_A5,  NOTE_G5,  NOTE_F5,

    NOTE_C6,  NOTE_G5,  NOTE_G5,  REST,     NOTE_C5, NOTE_D5,  NOTE_D5,
    NOTE_AS5, NOTE_A5,  NOTE_G5,  NOTE_F5,  NOTE_F5, NOTE_G5,  NOTE_A5,
    NOTE_G5,  NOTE_D5,  NOTE_E5,  NOTE_C6,  NOTE_C6, NOTE_F6,  NOTE_DS6,
    NOTE_CS6, NOTE_C6,  NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,  NOTE_C6};

static std::vector<uint32_t> durationsStarWars = {
    8, 8,  8,  2, 2, 8, 8, 8, 2,  4, 8,  8, 8, 2, 4, 8,
    8, 8,  2,  8, 8, 8, 2, 2, 8,  8, 8,  2, 4,

    8, 8,  8,  2, 4, 8, 8, 8, 2,  8, 16, 4, 8, 8, 8, 8,
    8, 8,  8,  8, 4, 8, 4, 8, 16, 4, 8,  8, 8, 8, 8,

    8, 16, 2,  8, 8, 4, 8, 8, 8,  8, 8,  8, 8, 8, 4, 8,
    4, 8,  16, 4, 8, 4, 8, 4, 8,  4, 8,  1};
