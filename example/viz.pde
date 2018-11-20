class Result {
  String name;
  int yeas;
  int nays;

  Result(String _name, int y, int n) {
    name = _name;
    yeas = y;
    nays = n;
  }
}

ArrayList<Result> results;

Table table;

void load() {
  table = loadTable("data.csv", "header,csv");
  println(table.getRowCount() + " total rows in table");

  results = new ArrayList<Result>();

  for (TableRow row : table.rows()) {
    String name = row.getString("name");
    int yeas = row.getInt("yeas");
    int nays = row.getInt("nays");

    Result r = new Result(name, yeas, nays);
    results.add(r);
  }
}


void viz(float t, float water, float wind, float solar) {
  background(255);
  
  if (t < 0.5) {
    viz1(t * 2, water, wind, solar);
  } else {
    viz2((t - 0.5) * 2, water, wind, solar);
  }
}

void viz1(float t, float water, float wind, float solar) {
  background(255);

  int i = 0;
  for (Result r : results) {
    float angle = 360.0 / results.size() * i;
    float distance = map(r.nays, 0, 2500 * t, 0, width/2);
    PVector pos = pointOnCircle(angle, distance);

    stroke(water * 255, wind * 255, solar * 255);
    line(width/2, height/2, width/2 + pos.x, height/2 + pos.y);

    i++;

    if (i > results.size()*t) {
      break;
    }
  }
}

void viz2(float t, float water, float wind, float solar) {
  background(255);

  int i = 0;
  for (Result r : results) {
    float angle = 360.0 / results.size() * i;
    float distance = map(r.yeas, 0, 2500 * t, 0, width/2);
    PVector pos = pointOnCircle(angle, distance);

    stroke(water * 255, wind * 255, solar * 255);
    line(width/2, height/2, width/2 + pos.x, height/2 + pos.y);

    i++;
  }
}
