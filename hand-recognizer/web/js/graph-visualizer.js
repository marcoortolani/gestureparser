$(document).ready(function(){
  $.ajaxSetup({ cache: false }); //disallows cachinge, so information should be new
  var firstTime = true;
  var names = [];
  var items = [];
  var previous = null;
  var current = null;
  window.setInterval(function(){
    $.getJSON( "../data.json", function( data ) {
      $("#hand-img").attr("src","file:///Users/Francesco/Documents/XcodeProjects/Kinect%20Hand%20Recognizer/data.jpg?date="+new Date());
      current = JSON.stringify(data);
      if((previous && current && previous !== current) || firstTime){
        $.each( data, function( key, val ) {
          //console.log(key+": "+val );
          names.push(key);
          items.push(val);
          firstTime = false;
        });
        $(function () {
              Highcharts.chart('graph-visualizer', {
                  title: {
                      text: 'Funzione Angolo Distanza',
                      x: -20 //center
                  },
                  subtitle: {
                      text: 'Features estratta tramine Kinect - Dev@DIID.UNIPA - Francesco Lanza ',
                      x: -20
                  },
                  xAxis: {
                      categories: items[0],
                      title:{
                        text: names[0]
                      }
                  },
                  yAxis: {
                      title: {
                          text: names[1]
                      },
                      plotLines: [{
                          value: 0,
                          width: 1,
                          color: '#808080'
                      }]
                  },
                  series: [{
                      name: 'Funzione '+names[0]+'-'+names[1],
                      data: items[1],
                      color: '#F44336'
                  }]
              });
          });//fine funzione Highcharts
          $(function () {
                Highcharts.chart('graph-samples', {
                    title: {
                        text: 'Funzione Angolo Distanza Campionata',
                        x: -20 //center
                    },
                    subtitle: {
                        text: 'Features estratta tramine Kinect - Dev@DIID.UNIPA - Francesco Lanza ',
                        x: -20
                    },
                    xAxis: {
                        categories: [],
                        title:{
                          text: names[0]
                        }
                    },
                    yAxis: {
                        title: {
                            text: names[1]
                        },
                        plotLines: [{
                            value: 0,
                            width: 1,
                            color: '#808080'
                        }]
                    },
                    series: [{
                        name: 'Funzione '+names[0]+'-'+names[1]+'('+names[2]+')',
                        data: items[2],
                        color: '#212121'
                    }]
                });
            });//fine funzione Highcharts
        }//if parentesi
        previous = current;
      });//fine getJSON
names=[];
items=[];
  },2000);
});
