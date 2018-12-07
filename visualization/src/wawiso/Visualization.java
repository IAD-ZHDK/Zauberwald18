package wawiso;

import processing.core.PApplet;

import java.util.ArrayList;

import static processing.core.PApplet.*;

public class Visualization {
  static final int DOME_RADIUS = 450;
  ArrayList<Particle> particlesWater = new ArrayList<>();
  ArrayList<Particle> particlesWind = new ArrayList<>();
  ArrayList<Particle> particlesSolar = new ArrayList<>();
  ArrayList<City> cities = new ArrayList<>();

  int particleNumSolar = 850; // 7
  int particleNumWind = 250; // 5
  int particleNumWater = 3900; // 5
  int cityNum = 23;

  float equatorRadius = 400;
  int particleSpiral = 0;

  Boolean ripple = false;
  Boolean vortex = false;
  Boolean falling = false;
  Boolean rotating = false;
  Boolean innerCircle = false;
  Boolean outerCircle = false;
  Boolean initializing = false;

  float water = 0;
  float wind = 0;
  float solar = 0;
  float t = 0;

  float waterTemp = 0;
  float windTemp = 0;
  float solarTemp = 0;

  int counterWater = 0;
  int counterWind = 0;
  int counterSolar = 0;
  private PApplet p;

  public Visualization(PApplet parent) {
    // set parent
    this.p = parent;
  }

  public void setup() {
    for (int i = 0; i < particleNumWater; i++) {
      particlesWater.add(
          new Particle(
              p,
              DOME_RADIUS * cos(0) + p.width / 2f,
              DOME_RADIUS * sin(0) + p.height / 2f,
              p.random(.5f, 2),
              p.random(0.05f, 0.1f),
              i,
              p.color(255)));
    }

    for (int i = 0; i < particleNumSolar; i++) {
      particlesSolar.add(
          new Particle(
              p,
              DOME_RADIUS * cos(0) + p.width / 2f,
              DOME_RADIUS * sin(0) + p.height / 2f,
              p.random(.5f, 2),
              p.random(0.05f, 0.1f),
              i,
              p.color(255)));
    }

    for (int i = 0; i < particleNumWind; i++) {
      particlesWind.add(
          new Particle(
              p,
              DOME_RADIUS * cos(0) + p.width / 2f,
              DOME_RADIUS * sin(0) + p.height / 2f,
              p.random(.5f, 2f),
              p.random(0.05f, 0.1f),
              i,
              p.color(255)));
    }

    for (City c : cities) {
      c.reset();
    }

    cities.add(new City(p, p.width / 2f + 180, p.height / 2f - 140, 150, 400)); // Zürich
    cities.add(new City(p, p.width / 2f - 70, p.height / 2f - 0, 103, 400)); // Bern
    cities.add(new City(p, p.width / 2f - 250, p.height / 2f + 120, 79, 400)); // Waadt
    cities.add(new City(p, p.width / 2f + 80, p.height / 2f - 160, 67, 400)); // Aargau
    cities.add(new City(p, p.width / 2f + 330, p.height / 2f - 160, 50, 400)); // stGallen
    cities.add(new City(p, p.width / 2f - 330, p.height / 2f + 210, 49, 400)); // Genf
    cities.add(new City(p, p.width / 2f + 110, p.height / 2f - 30, 40, 400)); // Luzern

    for (Particle _p : particlesWater) {
      falling = false;
      _p.stopped = false;
      _p.pos.x = p.random((p.width / 2f) - DOME_RADIUS, (p.width / 2f) + DOME_RADIUS);
      _p.pos.y = p.random((p.height / 2f) - DOME_RADIUS, (p.height / 2f) + DOME_RADIUS);
    }

    for (Particle _p : particlesWind) {
      falling = false;
      _p.stopped = false;
      _p.pos.x = p.random(p.width);
      _p.pos.y = p.random(p.height);
    }

    for (Particle _p : particlesSolar) {
      falling = false;
      _p.stopped = false;
      _p.pos.x = p.random(p.width);
      _p.pos.y = p.random(p.height);
    }
  }

  public void draw(float t, float water, float wind, float solar) {
    p.stroke(255);
    p.fill(255);

    p.fill(0, 120);
    p.rect(0, 0, p.width, p.height);

    for (Particle _p : particlesSolar) {
      if (solar > 0) {
        _p.colorE = p.color(255, 255, 255 - solar * 255);
      }
      for (City c : cities) {
        if (_p.attracting) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, c.size / 2);
        if (_p.repelling) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, 0);
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }

      if (_p.flowing) _p.flow();
      if (_p.trembling) _p.tremble();
      if (falling) _p.fall();

      if (initializing && particleSpiral > particleNumSolar - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      if (outerCircle) {
        _p.flowing = false;
        _p.repelling = true;
        _p.magnet(p.width / 2f, p.height / 2f, equatorRadius, 10);
        _p.circleRotation(equatorRadius);
      } else {
        _p.flowing = true;
      }

      if (rotating) {
        _p.flowing = false;
        _p.circleRotation(_p.distanceFromCenter);
      } else {
        _p.flowing = true;
      }

      if (ripple) {
        _p.ripple();
      } else {
        _p.rippling = false;
        _p.ripplingSize = 0;
      }

      if (innerCircle) {
        _p.flowing = false;
        _p.circleRotation(equatorRadius);
      }

      if (initializing) {
        // p.emit();
        _p.run();
        initializing = false;
      }
      _p.run();
    }
    for (Particle _p : particlesWater) {
      if (water > 0) {
        _p.colorE = p.color(255 - water * 255, 255, 255);
      }
      for (City c : cities) {
        if (_p.attracting) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, c.size / 2);
        if (_p.repelling) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, 0);
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }

      if (_p.flowing) _p.flow();
      if (_p.trembling) _p.tremble();
      if (falling) _p.fall();

      if (initializing && particleSpiral > particleNumWater - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      if (outerCircle) {
        _p.flowing = false;
        _p.repelling = true;
        _p.magnet(p.width / 2f, p.height / 2f, equatorRadius, 10);
        _p.circleRotation(equatorRadius);
      } else {
        _p.flowing = true;
      }

      if (rotating) {
        _p.flowing = false;
        _p.circleRotation(_p.distanceFromCenter);
      } else {
        _p.flowing = true;
      }

      if (ripple) {
        _p.ripple();
      } else {
        _p.rippling = false;
        _p.ripplingSize = 0;
      }

      if (innerCircle) {
        _p.flowing = false;
        _p.circleRotation(equatorRadius);
      }

      if (initializing) {
        // p.emit();
        _p.run();
        initializing = false;
      }
      _p.run();
    }

    for (Particle _p : particlesWind) {
      if (wind > 0) {
        _p.colorE = p.color(255, 255 - wind * 255, 255);
      }
      for (City c : cities) {
        if (_p.attracting) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, c.size / 2);
        if (_p.repelling) _p.magnet(c.pos.x, c.pos.y, c.magnetRadius, 0);
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }
      if (_p.flowing) _p.flow();
      if (_p.trembling) _p.tremble();
      if (falling) _p.fall();

      if (initializing && particleSpiral > particleNumWind - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      if (outerCircle) {
        _p.flowing = false;
        _p.repelling = true;
        _p.magnet(p.width / 2f, p.height / 2f, equatorRadius, 10);
        _p.circleRotation(equatorRadius);
      } else {
        _p.flowing = true;
      }

      if (rotating) {
        _p.flowing = false;
        _p.circleRotation(_p.distanceFromCenter);
      } else {
        _p.flowing = true;
      }

      if (ripple) {
        _p.ripple();
      } else {
        _p.rippling = false;
        _p.ripplingSize = 0;
      }

      if (innerCircle) {
        _p.flowing = false;
        _p.circleRotation(equatorRadius);
      }

      if (initializing) {
        // p.emit();
        _p.run();
        initializing = false;
      }

      _p.run();
    }

    // for (City c : cities) {
    //  c.display();
    // }

    // if (ripple) {
    //  rippleRadius += 5;
    //  if (rippleRadius > DOME_RADIUS) ripple = false;
    // }

    for (City c : cities) {
      c.drawCity(water, solar, wind);
    }
  }
}
