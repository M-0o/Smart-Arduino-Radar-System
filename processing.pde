import processing.serial.*;

Serial myPort;

float angle = 90;
float[] dists = {400, 400, 400, 400};

float smoothAngle = 90;
float r;

boolean anyTarget = false;
float sigOffset = 0;

int[] offset = {0, 180, 90, 270};

void setup() {
  size(1200, 850);
  smooth();

  r = height * 0.65;

  println("AVAILABLE PORTS:");
  printArray(Serial.list());

  String portName = Serial.list()[0];

  println("CONNECTED TO: " + portName);

  myPort = new Serial(this, portName, 115200);
  myPort.clear();
  myPort.bufferUntil('.');
}

void draw() {
  background(3, 10, 3);

  smoothAngle += (angle - smoothAngle) * 0.2;

  anyTarget =
    dists[0] >= 5 && dists[0] <= 10 ||
    dists[1] >= 5 && dists[1] <= 10 ||
    dists[2] >= 5 && dists[2] <= 10 ||
    dists[3] >= 5 && dists[3] <= 10;

  pushMatrix();
  translate(width/2 - 100, height/2 - 40);

  drawRadarGrid();
  drawScanLines();
  drawTargets();

  popMatrix();

  drawUI();
  drawSimulinkWindow();
  drawCombatControls();
}

void drawRadarGrid() {
  noFill();

  for (int i = 1; i <= 5; i++) {
    stroke(0, 255, 0, 35 * i);
    ellipse(0, 0, (r / 5) * i, (r / 5) * i);
  }

  stroke(0, 255, 0, 60);
  line(-r/2, 0, r/2, 0);
  line(0, -r/2, 0, r/2);
}

void drawScanLines() {
  color[] cols = {
    color(0, 255, 0),
    color(0, 150, 255),
    color(255, 255, 0),
    color(200, 0, 255)
  };

  float localAngle = smoothAngle - 90;

  for (int i = 0; i < 4; i++) {
    stroke(cols[i], 120);
    strokeWeight(3);

    float lineAngle = localAngle + offset[i];

    float endX = r/2 * cos(radians(-lineAngle));
    float endY = r/2 * sin(radians(-lineAngle));

    line(0, 0, endX, endY);
  }
}

void drawTargets() {
  color[] cols = {
    color(0, 255, 0),
    color(0, 150, 255),
    color(255, 255, 0),
    color(200, 0, 255)
  };

  float localAngle = angle - 90;

  for (int i = 0; i < 4; i++) {
    if (dists[i] >= 5 && dists[i] <= 10) {
      float pix = map(dists[i], 5, 10, 0, r/2);

      float targetAngle = localAngle + offset[i];

      float x = pix * cos(radians(-targetAngle));
      float y = pix * sin(radians(-targetAngle));

      fill(255, 0, 0, 220);
      noStroke();
      ellipse(x, y, 24, 24);

      fill(255);
      textSize(13);
      text(int(dists[i]) + "cm", x + 15, y);

      stroke(cols[i], 180);
      strokeWeight(2);
      line(0, 0, x, y);
    }
  }
}

void drawUI() {
  fill(0, 20, 0, 230);
  stroke(0, 255, 0);
  rect(width - 260, 20, 240, 450, 15);

  fill(0, 255, 0);
  textAlign(CENTER);
  textSize(20);
  text("CONTROL PANEL", width - 140, 55);

  textAlign(LEFT);
  textSize(22);

  for (int i = 0; i < 4; i++) {
    fill(255);

    String value = "---";

    if (dists[i] >= 5 && dists[i] <= 10) {
      value = int(dists[i]) + "cm";
    }

    text("SNS " + (i + 1) + ": " + value, width - 240, 120 + (i * 90));
  }

  fill(0, 255, 0);
  textSize(14);
  text("Servo Angle: " + int(angle) + "°", width - 240, 530);
  text("Range: 5 - 10 cm", width - 240, 555);
}

void drawSimulinkWindow() {
  pushMatrix();
  translate(40, height - 160);

  fill(0, 15, 0);
  stroke(0, 255, 0);
  rect(0, 0, 450, 120, 10);

  fill(0, 255, 0);
  textSize(12);
  text("REAL-TIME SIGNAL", 10, -10);

  noFill();
  stroke(0, 255, 0, 200);
  strokeWeight(2);

  beginShape();

  for (int x = 0; x < 450; x++) {
    float y =
      80 +
      (anyTarget && x > 180 && x < 270
      ? sin((x + sigOffset) * 0.6) * 35
      : random(-2, 2));

    vertex(x, y);
  }

  endShape();

  sigOffset -= 5;

  popMatrix();
}

void drawCombatControls() {
  fill(0, 15, 0, 245);
  stroke(anyTarget ? color(255, 0, 0) : color(0, 255, 0));
  rect(width - 260, 490, 240, 330, 15);

  if (anyTarget) {
    if (
      mouseX > width - 240 &&
      mouseX < width - 40 &&
      mouseY > 540 &&
      mouseY < 640
    ) {
      fill(255, 0, 0);

      if (mousePressed) {
        myPort.write('F');
        resetTargets();
      }

    } else {
      fill(180, 0, 0);
    }

    rect(width - 240, 540, 200, 100, 15);

    fill(255);
    textAlign(CENTER);
    textSize(26);
    text("FIRE", width - 140, 600);

    if (
      mouseX > width - 240 &&
      mouseX < width - 40 &&
      mouseY > 670 &&
      mouseY < 760
    ) {
      fill(100);

      if (mousePressed) {
        myPort.write('C');
        resetTargets();
      }

    } else {
      fill(60);
    }

    rect(width - 240, 670, 200, 90, 15);

    fill(255);
    textSize(20);
    text("CANCEL", width - 140, 725);

  } else {
    fill(0, 255, 0, 150);
    textAlign(CENTER);
    textSize(22);
    text("SCANNING...", width - 140, 660);
  }
}

void resetTargets() {
  for (int i = 0; i < 4; i++) {
    dists[i] = 400;
  }

  anyTarget = false;
}

void serialEvent(Serial p) {
  try {
    String val = p.readStringUntil('.');

    if (val != null) {
      val = trim(val);
      val = val.replace(".", "");

      println("DATA: " + val);

      String[] list = split(val, ',');

      if (list.length == 5) {
        angle = float(list[0]);

        for (int i = 0; i < 4; i++) {
          dists[i] = float(list[i + 1]);
        }

      } else {
        println("FORMAT ERROR: " + val);
      }
    }

  } catch (Exception e) {
    println("SERIAL ERROR:");
    println(e);
  }
}
