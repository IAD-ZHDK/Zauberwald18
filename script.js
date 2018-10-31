import '@babel/polyfill';

import * as d3 from 'd3';
import mqtt from 'mqtt';
import copy from 'deep-copy';

/* Environment */

let width = window.innerWidth;
let height = window.innerHeight;

/* Keys */

let keys = ['solar1', 'solar2', 'wind1', 'wind2', 'water1', 'water2'];

/* Margin */

let margin = 100;

/* SVG */

const svg = d3.select('svg')
    .attr('width', width)
    .attr('height', height);

/* Layers */

let gArea = svg.append("g");
let gYAxis = svg.append("g");
let gLegend = svg.append("g");

function viz(rawData) {
  /* Data */

  let stackData = d3.stack().keys(keys);

  let data = stackData(rawData);

  /* Scales */

  let x = d3.scaleLinear()
      .domain(d3.extent(rawData, (_, i) => i))
      .range([margin, width - margin]);

  let y = d3.scaleLinear()
      .domain([0, d3.max(rawData, d => keys.reduce((v, k) => v + d[k], 0))])
      .range([height - margin, margin]);

  let color = d3.scaleOrdinal(d3.schemeCategory10).domain(data.map(d => d.key));

  /* Area Generator */

  let area = d3.area()
      .x((_, i) => x(i))
      .y0(d => y(d[0]))
      .y1(d => y(d[1]));

  /* Legend */

  const g = gLegend
      .attr("transform", `translate(${margin},${margin-40})`)
      .attr("font-family", "sans-serif")
      .attr("font-size", 10)
      .selectAll("g")
      .data(color.domain().slice())
      .enter().append("g")
      .attr("transform", (d, i) => `translate(${i * 100}, 10)`);

  g.append("rect")
      .attr("width", 19)
      .attr("height", 19)
      .attr("fill", color);

  g.append("text")
      .attr("x", 30)
      .attr("y", 9.5)
      .attr("dy", "0.35em")
      .text(d => d);

  /* Axis */

  gYAxis
      .attr("transform", `translate(${width - margin},0)`)
      .call(d3.axisRight(y))
      .call(g => g.select(".domain").remove());

  /* Viz */

  let areaSet = gArea
      .selectAll("path")
      .data(data);

  areaSet.enter().append("path");

  areaSet.attr("fill", d => color(d.key))
      .attr("d", d => area(d))
      .append("title")
      .text(d => d.key);
}

/* Data */

let store = {};

keys.forEach(k => {
  store[k] = 0;
});

let data = [];

for (let i=0; i<200; i++) {
  data.push(copy(store));
}

/* Snapshooter */

function snapshot() {
  data.push(copy(store));

  if (data.length > 200) {
    data.shift();
  }

  viz(data);
}

setInterval(snapshot, 100);

/* Input */

let client  = mqtt.connect('wss://zw18zw18:zw18zw18@broker.shiftr.io');

client.on('connect', () => {
  client.subscribe('#');
});

client.on('message', (topic, message) => {
  if (keys.indexOf(topic) >= 0) {
    store[topic] = parseFloat(message);
  }
});
