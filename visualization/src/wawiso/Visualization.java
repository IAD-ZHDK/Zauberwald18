package wawiso;

import processing.core.PApplet;

import java.util.ArrayList;

import static processing.core.PApplet.*;

public class Visualization {
  static final int DOME_RADIUS = 450;
  private ArrayList<Particle> particlesWater = new ArrayList<>();
  private ArrayList<Particle> particlesWind = new ArrayList<>();
  private ArrayList<Particle> particlesSolar = new ArrayList<>();
  private ArrayList<City> cities = new ArrayList<>();

  private int particleNumSolar = 850; // 7
  private int particleNumWind = 250; // 5
  private int particleNumWater = 3900; // 5

  private int particleSpiral = 0;

  private Boolean initializing = false;

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

    cities.add(new City(p, p.width / 2f + 180, p.height / 2f - 140, 150)); // Zürich
    cities.add(new City(p, p.width / 2f - 70, p.height / 2f - 0, 103)); // Bern
    cities.add(new City(p, p.width / 2f - 250, p.height / 2f + 120, 79)); // Waadt
    cities.add(new City(p, p.width / 2f + 80, p.height / 2f - 160, 67)); // Aargau
    cities.add(new City(p, p.width / 2f + 330, p.height / 2f - 160, 50)); // St. Gallen
    cities.add(new City(p, p.width / 2f - 330, p.height / 2f + 210, 49)); // Genf
    cities.add(new City(p, p.width / 2f + 110, p.height / 2f - 30, 40)); // Luzern

    for (Particle _p : particlesWater) {
      _p.pos.x = p.random((p.width / 2f) - DOME_RADIUS, (p.width / 2f) + DOME_RADIUS);
      _p.pos.y = p.random((p.height / 2f) - DOME_RADIUS, (p.height / 2f) + DOME_RADIUS);
    }

    for (Particle _p : particlesWind) {
      _p.pos.x = p.random(p.width);
      _p.pos.y = p.random(p.height);
    }

    for (Particle _p : particlesSolar) {
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
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }

      if (_p.flowing) _p.flow();

      if (initializing && particleSpiral > particleNumSolar - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      _p.flowing = true;
      _p.ripplingSize = 0;

      if (initializing) {
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
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }

      if (_p.flowing) _p.flow();

      if (initializing && particleSpiral > particleNumWater - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      _p.flowing = true;
      _p.ripplingSize = 0;

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
        float distance = dist(_p.pos.x, _p.pos.y, c.pos.x, c.pos.y);
        if (distance <= c.size / 2) {
          _p.position();
        }
      }
      if (_p.flowing) _p.flow();

      if (initializing && particleSpiral > particleNumWind - 10) {
        initializing = false;
        _p.flowing = true;
      }

      if (p.millis() % ((_p.index * 5) + 1) == 0 && !_p.start) {
        _p.start = true;
      }

      _p.flowing = true;
      _p.ripplingSize = 0;

      if (initializing) {
        _p.run();
        initializing = false;
      }

      _p.run();
    }

    for (City c : cities) {
      c.drawCity(water, solar, wind);
    }
  }
}
