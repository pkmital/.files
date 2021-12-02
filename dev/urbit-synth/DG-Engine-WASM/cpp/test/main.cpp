#include "AudioFile.h"
#include "Datagarden.h"
#include <vector>

std::vector<std::string> sounds = {"Guitar 2", "Guitar 1", "Harp",
                                   "Shakuhachi"};
// std::vector<std::string> all_sounds = {
//     "Celeste",  "Choir 1", "Choir 2",    "Crickets", "Electric Piano",
//     "Flute",    "Gamelan", "Glassy",     "Gong",     "Guitar 1",
//     "Guitar 2", "Harp",    "Organ 1",    "Organ 2",  "Pan Pipe",
//     "Piano 1",  "Piano 2", "Pixie Dust", "Quartet",  "Shakuhachi",
//     "Shimmer",  "Synth 1", "Synth 2",    "Synth 3"};

int main() {
  printf("building sound list!\n");
  DGAudioEngine dge;
  nlohmann::json json_state =
      dge.readJSONintoState("dmt/Standard/Points on Waves.dmt");
  std::map<std::string, std::vector<std::string>> sample_paths;
  for (std::vector<std::string>::iterator it = sounds.begin();
       it != sounds.end(); it++) {
    printf("sound: %s\n", it->c_str());
    sample_paths[*it].push_back("audio/" + *it + ".wav");
  }
  printf("initializing engine!\n");
  dge.initialize(json_state, sample_paths);

  printf("starting engine!\n");
  dge.start();
  dge.startPlayback();
  dge.mute(false);

  printf("writing midi message!\n");
  BSMidiMessage m;
  m.message[0] = 0x90;
  m.message[1] = 60;
  m.message[2] = 127;
  m.scheduled = true;
  m.timestamp = 1000;
  dge.scheduleMIDIMessage(m);

  printf("building data\n");
  int sample_rate = 44100;
  float seconds_to_record = 5;
  int frame_size = 512;
  int hop_size = 512;
  int n_hops = sample_rate * seconds_to_record / hop_size;
  int n_channels = 2;

  // this is stored as a 2d split vector
  std::vector<std::vector<float>> audio_buffer;
  audio_buffer.resize(2);
  audio_buffer[0].resize(sample_rate * seconds_to_record);
  audio_buffer[1].resize(sample_rate * seconds_to_record);

  // this is stored as a 1d interleaved vector
  std::vector<float> frame_data;
  frame_data.resize(frame_size * n_channels);

  double current_time = 0.0;
  printf("rendering audio!\n");
  for (int i = 0; i < n_hops; i++) {
    current_time = (float) i * (float) hop_size / (float) sample_rate * 1000.0;
    printf("%f\n", current_time);
    dge.renderAudio(&(frame_data[0]), frame_size, n_channels, current_time);
    if (i == 10) {
      BSMidiMessage m1 = midiMessageForNoteOn(60);
      dge.midi(&m1);
      BSMidiMessage m2 = midiMessageForNoteOn(60);
      dge.midi(&m2);
      BSMidiMessage m3 = midiMessageForNoteOn(60);
      dge.midi(&m3);
      BSMidiMessage m4 = midiMessageForCC(80, 127);
      dge.midi(&m4);
    }

    for (int sample = 0; sample < frame_size; sample++) {
      int current_frame = i * hop_size;
      audio_buffer[0][current_frame + sample] += frame_data[n_channels * sample + 0];
      audio_buffer[1][current_frame + sample] += frame_data[n_channels * sample + 1];
    }
  }

  printf("writing to wave!\n");
  AudioFile<float> af;
  af.setNumChannels(2);
  af.setNumSamplesPerChannel(sample_rate * seconds_to_record);
  af.setBitDepth(24);
  af.setSampleRate(sample_rate);
  af.setAudioBuffer(audio_buffer);
  af.printSummary();
  af.save("output.wav");

  printf("okay!\n");
}
