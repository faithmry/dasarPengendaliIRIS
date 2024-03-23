var odom_robot = [0, 0, 0];

var ws_robot = new ROSLIB.Ros({
  url: "ws://" + window.location.hostname + ":9090",
});

ws_robot.on("connection", function () {
  console.log("Connection made Robot 1!");
  is_robot_connected = 1;
});

ws_robot.on("close", function () {
  console.log("Connection closed Robot 1!");
  is_robot_connected = 0;
});

var sub_odom_robot = new ROSLIB.Topic({
  ros: ws_robot,
  name: "/robot/final_odom",
  messageType: "geometry_msgs/Pose2D",
});

// ---------------------------------------------------

var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");

const WIDTH = 1200;
const HEIGHT = 800;

let regional = 0;

function rx(x) {
  return x + 100;
}

function ry(y) {
  return y + 100;
}

sub_odom_robot.subscribe(function (message) {
  odom_robot[0] = message.x;
  odom_robot[1] = message.y;
  odom_robot[2] = message.theta;

  console.log("x:" + odom_robot[0] + " y:" + odom_robot[1]);

  // ctx.moveTo(rx(odom_robot[0]), odom_robot[1]);
  // ctx.arc(rx(odom_robot[0]), odom_robot[1], 3, 0, 2 * Math.PI);
  // ctx.stroke();
  drawPose(message);
});

drawPose();

function drawPose(pose) {
  ctx.clearRect(0, 0, c.width, c.height);
  ctx.save();

  if (regional) {
    c.width = 1000;
    c.height = 800;

    ctx.moveTo(100, 100);
    ctx.lineTo(100, 700);
    ctx.lineTo(900, 700);
    ctx.lineTo(900, 100);
    ctx.lineTo(100, 100);

    ctx.moveTo(400, 700);
    ctx.lineTo(400, 250);

    ctx.moveTo(600, 700);
    ctx.lineTo(600, 250);

    ctx.moveTo(600, 700);
    ctx.lineTo(600, 250);

    ctx.moveTo(300, 100);
    ctx.lineTo(300, 250);
    ctx.lineTo(700, 250);
    ctx.lineTo(700, 100);

    ctx.moveTo(500, 700);
    ctx.arc(500, 700, 100, 0, 1 * Math.PI, true);

    ctx.moveTo(100, 100);
    ctx.arc(100, 100, 50, 0, 0.5 * Math.PI);

    ctx.moveTo(900, 100);
    ctx.arc(900, 100, 50, 0.5 * Math.PI, 1 * Math.PI);

    ctx.stroke();
    //
  } else {
    //
    let back = new Path2D();
    back.moveTo(rx(-100), ry(-100));
    back.lineTo(rx(1300), ry(-100));
    back.lineTo(rx(1300), ry(900));
    back.lineTo(rx(-100), ry(900));
    back.lineTo(rx(-100), ry(-100));
    // back.stroke();
    back.closePath();

    ctx.fillStyle = "green";
    ctx.fill(back, "evenodd");

    let lap = new Path2D();
    lap.moveTo(rx(0), ry(0));
    lap.lineTo(rx(1200), ry(0));
    lap.lineTo(rx(1200), ry(800));
    lap.lineTo(rx(0), ry(800));
    lap.lineTo(rx(0), ry(0));
    // lap.stroke();
    lap.closePath();

    ctx.fillStyle = "green";
    ctx.fill(lap, "evenodd");

    ctx.lineWidth = 2;
    ctx.strokeStyle = "white";
    ctx.moveTo(rx(0), ry(0));
    ctx.lineTo(rx(1200), ry(0));
    ctx.lineTo(rx(1200), ry(800));
    ctx.lineTo(rx(0), ry(800));
    ctx.lineTo(rx(0), ry(0));

    // garis tengah
    ctx.moveTo(rx(1200 / 2), ry(0));
    ctx.lineTo(rx(1200 / 2), ry(800));

    // lingkaran tengah besar
    ctx.moveTo(rx(1200 / 2), ry(800 / 2));
    ctx.arc(rx(1200 / 2), ry(800 / 2), 260 / 2, 0.5 * Math.PI, 1.5 * Math.PI);
    ctx.arc(rx(1200 / 2), ry(800 / 2), 260 / 2, 1.5 * Math.PI, 2.5 * Math.PI);

    // lingkaran tengah kecil
    ctx.moveTo(rx(1200 / 2), ry(800 / 2));
    ctx.arc(rx(1200 / 2), ry(800 / 2), 10, 0, 2 * Math.PI);

    //lingkaran corner
    ctx.moveTo(rx(WIDTH), ry(HEIGHT));
    ctx.arc(rx(WIDTH), ry(HEIGHT), 50, 1 * Math.PI, 1.5 * Math.PI);

    ctx.moveTo(rx(0), ry(HEIGHT));
    ctx.arc(rx(0), ry(HEIGHT), 50, 1.5 * Math.PI, 2 * Math.PI);

    ctx.moveTo(rx(0), ry(0));
    ctx.arc(rx(0), ry(0), 50, 2 * Math.PI, 2.5 * Math.PI);

    ctx.moveTo(rx(WIDTH), ry(0));
    ctx.arc(rx(WIDTH), ry(0), 50, 0.5 * Math.PI, 1 * Math.PI);

    // kotak gawang
    ctx.moveTo(rx(0), ry(400 - 100 - 50));
    ctx.lineTo(rx(50), ry(400 - 100 - 50));
    ctx.lineTo(rx(50), ry(400 + 100 + 50));
    ctx.lineTo(rx(0), ry(400 + 100 + 50));

    ctx.moveTo(rx(WIDTH), ry(400 - 100 - 50));
    ctx.lineTo(rx(WIDTH - 50), ry(400 - 100 - 50));
    ctx.lineTo(rx(WIDTH - 50), ry(400 + 100 + 50));
    ctx.lineTo(rx(WIDTH), ry(400 + 100 + 50));

    // kotak penalti
    ctx.moveTo(rx(0), ry(400 - 100 - 150));
    ctx.lineTo(rx(180), ry(400 - 100 - 150));
    ctx.lineTo(rx(180), ry(400 + 100 + 150));
    ctx.lineTo(rx(0), ry(400 + 100 + 150));

    ctx.moveTo(rx(WIDTH), ry(400 - 100 - 150));
    ctx.lineTo(rx(WIDTH - 180), ry(400 - 100 - 150));
    ctx.lineTo(rx(WIDTH - 180), ry(400 + 100 + 150));
    ctx.lineTo(rx(WIDTH), ry(400 + 100 + 150));

    // kotak gawang belakang
    ctx.moveTo(rx(0), ry(400 - 100));
    ctx.lineTo(rx(-25), ry(400 - 100));
    ctx.lineTo(rx(-25), ry(400 + 100));
    ctx.lineTo(rx(0), ry(400 + 100));

    ctx.moveTo(rx(WIDTH + 0), ry(400 - 100));
    ctx.lineTo(rx(WIDTH + 25), ry(400 - 100));
    ctx.lineTo(rx(WIDTH + 25), ry(400 + 100));
    ctx.lineTo(rx(WIDTH + 0), ry(400 + 100));

    ctx.stroke();

    for (let i = 1; i <= 3; i++) {
      //
      let y = ry(i * 200);

      ctx.moveTo(rx(200), y);
      ctx.arc(rx(200), y, 3, 0, 2 * Math.PI);
      ctx.moveTo(rx(WIDTH / 2), y);
      ctx.arc(rx(WIDTH / 2), y, 3, 0, 2 * Math.PI);
      ctx.moveTo(rx(WIDTH - 200), y);
      ctx.arc(rx(WIDTH - 200), y, 3, 0, 2 * Math.PI);
      ctx.stroke();
    }

    ctx.stroke();
  }

  // robot
  ctx.rotate(pose.theta);
  ctx.fillStyle = "blue";
  ctx.fillRect(odom_robot[0], odom_robot[1], 52, 52); // kotak robot
  ctx.restore();
}
