#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// =======================
// PIN SETUP
// =======================
const int trigPin = 9;
const int echoPin = 10;

// DFPlayer (RX, TX)
SoftwareSerial mySerial(12, 11); 
DFRobotDFPlayerMini dfPlayer;

// =======================
// VARIABEL
// =======================
bool sudahPutar = false;

// =======================
// SETUP
// =======================
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Inisialisasi DFPlayer...");

  if (!dfPlayer.begin(mySerial)) {
    Serial.println("DFPlayer gagal!");
    Serial.println("Cek kabel & SD Card");
    while (true); // berhenti total
  }

  Serial.println("DFPlayer siap!");
  dfPlayer.volume(20); // 0 - 30
}

// =======================
// LOOP
// =======================
void loop() {
  long duration;
  int distance;

  // --- Kirim trigger ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // --- Baca echo (pakai timeout biar tidak hang) ---
  duration = pulseIn(echoPin, HIGH, 30000);

  // --- Konversi ke cm ---
  distance = duration * 0.034 / 2;

  // --- Debug ---
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // =======================
  // LOGIC AUDIO
  // =======================
  if (distance > 0 && distance <= 10) {
    if (!sudahPutar) {
      Serial.println("Objek dekat! Putar suara...");
      dfPlayer.play(1); // file 0001.mp3
      sudahPutar = true;
    }
  } else {
    // Reset kalau objek menjauh
    sudahPutar = false;
  }

  delay(100); // delay kecil saja
}
