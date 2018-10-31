import * as d3 from 'd3';

/* Environment */

let width = window.innerWidth;
let height = window.innerHeight;

/* Margin */

let margin = 100;

/* SVG */

const svg = d3.select('#viz')
    .attr('width', width)
    .attr('height', height);

/* Layers */

let gArea = svg.append("g");
let gXAxis = svg.append("g");
let gYAxis = svg.append("g");
let gLegend = svg.append("g");

function update() {
  /* Data */

  let rawData = [
    {date: new Date(2015, 0, 1), solar:  640, wind:  960, water: 640},
    {date: new Date(2015, 1, 1), solar: 3840, wind: 1920, water: 960},
    {date: new Date(2015, 2, 1), solar: 1600, wind: 1440, water: 960},
    {date: new Date(2015, 3, 1), solar:  320, wind:  480, water: 640}
  ];

  let stackData = d3.stack().keys(["solar", "wind", "water"]);

  let data = stackData(rawData);

  /* Scales */

  let x = d3.scaleTime()
      .domain(d3.extent(rawData, d => d.date))
      .range([margin, width - margin]);

  let y = d3.scaleLinear()
      .domain([0, d3.max(rawData, d => d.solar + d.wind + d.water)])
      .range([height - margin, margin]);

  let color = d3.scaleOrdinal(d3.schemeCategory10).domain(data.map(d => d.key));

  /* Area Generator */

  let area = d3.area()
      .x(d => x(d.data.date))
      .y0(d => y(d[0]))
      .y1(d => y(d[1]));

  /* Legend */

  const g = gLegend
      .attr("transform", `translate(${margin},${margin})`)
      .attr("font-family", "sans-serif")
      .attr("font-size", 10)
      .selectAll("g")
      .data(color.domain().slice().reverse())
      .enter().append("g")
      .attr("transform", (d, i) => `translate(0,${i * 25})`);

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

  gXAxis
      .attr("transform", `translate(0,${height - margin})`)
      .call(d3.axisBottom(x).ticks(width / 80).tickSizeOuter(0));

  gYAxis
      .attr("transform", `translate(${width - margin},0)`)
      .call(d3.axisRight(y))
      .call(g => g.select(".domain").remove());

  /* Viz */

  gArea
      .selectAll("path")
      .data(data)
      .enter().append("path")
      .attr("fill", d => color(d.key))
      .attr("d", d => area(d))
      .append("title")
      .text(d => d.key);
}

update();
