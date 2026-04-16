const char WEB_CONTROL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>Remote Controller</title>
<style>
:root {
  --neon: #00f2ff;
  --neon-glow: 0 0 15px rgba(0, 242, 255, 0.7), 0 0 30px rgba(0, 242, 255, 0.3);
  --ps-white: #f5f5f7;
  --bg-dark: #050505;
}

html, body {
  margin: 0; padding: 0; width: 100%; height: 100%;
  background: var(--bg-dark); overflow: hidden; touch-action: none;
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  display: flex; align-items: center; justify-content: center;
}

/* KHUNG TAY CẦM PS5 */
.ps-controller {
  position: relative;
  width: 95vw;
  height: 82dvh;
  max-width: 1000px;
  max-height: 550px;
  background: var(--ps-white);
  border-radius: 90px 90px 120px 120px;
  box-shadow: 
    0 50px 100px rgba(0,0,0,0.9), 
    0 20px 40px rgba(0,0,0,0.6),
    inset 0 -15px 30px rgba(0,0,0,0.1),
    inset 0 5px 15px rgba(255,255,255,1);
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0 40px;
  border-bottom: 10px solid #cfcfcf;
}

.ps-controller::before, .ps-controller::after {
  content: "";
  position: absolute;
  bottom: -40px;
  width: 280px;
  height: 420px;
  background: var(--ps-white);
  z-index: -1;
  border-radius: 130px;
}
.ps-controller::before { left: -10px; transform: rotate(20deg); box-shadow: -15px 30px 50px rgba(0,0,0,0.5); }
.ps-controller::after { right: -10px; transform: rotate(-20deg); box-shadow: 15px 30px 50px rgba(0,0,0,0.5); }

/* TOUCHPAD GIỮA */
.touchpad {
  position: absolute;
  top: 0; left: 50%;
  transform: translateX(-50%);
  width: 30%;
  height: 45%; 
  background: linear-gradient(145deg, #1e1e1e, #0a0a0a);
  border-radius: 0 0 50px 50px;
  border: 2px solid #222;
  box-shadow: 0 0 20px rgba(0, 242, 255, 0.4), inset 0 0 30px rgba(0,0,0,1);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  z-index: 10;
  padding: 20px;
}

/* CSS CHO THANH TRƯỢT TỐC ĐỘ */
.speed-ctrl-container {
  width: 100%;
  margin-top: 15px;
  text-align: center;
}
.speed-slider {
  -webkit-appearance: none;
  width: 100%;
  height: 8px;
  background: #333;
  border-radius: 5px;
  outline: none;
}
.speed-slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 20px;
  height: 20px;
  background: var(--neon);
  border-radius: 50%;
  cursor: pointer;
  box-shadow: var(--neon-glow);
}
#speedValueDisplay {
  color: var(--neon);
  font-weight: bold;
  font-size: 18px;
  margin-top: 5px;
  text-shadow: 0 0 5px var(--neon);
}

.led-strip {
  position: absolute;
  top: 6px; width: 88%; height: 4px;
  background: var(--neon);
  border-radius: 10px;
  box-shadow: var(--neon-glow);
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0% { opacity: 0.8; box-shadow: 0 0 10px var(--neon); }
  50% { opacity: 1; box-shadow: 0 0 25px var(--neon); }
  100% { opacity: 0.8; box-shadow: 0 0 10px var(--neon); }
}

.touchpad h2 {
  color: #fff;
  font-size: 16px;
  letter-spacing: 4px;
  margin-bottom: 20px;
  text-transform: uppercase;
  text-shadow: 0 0 10px var(--neon);
}

.instruction-box {
  text-align: left;
  color: #888;
  font-size: 9px;
  line-height: 2;
  font-weight: bold;
  letter-spacing: 0.5px;
}

.instruction-box span {
  color: #bbb;
  border-bottom: 1px solid #333;
  margin: 0 4px;
}

/* ZONE CHUNG CHO JOYSTICK VÀ BUTTONS */
.bnt-zone {
  margin-top: 10px;
  display: flex;
  flex-direction: column;
  align-items: center;
  z-index: 20;
}

/* CỤM NÚT PHẢI */
.fb-button-container {
  width: 150px;
  height: 200px;
  display: flex;
  flex-direction: column;
  gap: 20px;
  justify-content: space-around;
  align-items: center;
}

.fb-bnt {
  width: 80px;
  height: 80px;
  background: radial-gradient(circle at 35% 35%, #4a4a4a 0%, #050505 100%);
  border-radius: 20px;
  border: 2px solid #222;
  color: var(--neon);
  font-size: 30px;
  font-weight: bold;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 10px 20px rgba(0,0,0,0.5);
  cursor: pointer;
  user-select: none;
  transition: 0.1s;
  text-shadow: 0 0 10px var(--neon);
}

.fb-bnt:active, .fb-bnt.active {
  transform: scale(0.9);
  box-shadow: inset 0 5px 10px rgba(0,0,0,0.8), 0 0 20px var(--neon);
  border-color: var(--neon);
}

/* CỤM NÚT  */
.button-container {
  width: 200px;
  height: 150px;
  display: flex;
  gap: 20px;
  justify-content: space-around;
  align-items: center;
}

.lf-bnt {
  width: 80px;
  height: 80px;
  background: radial-gradient(circle at 35% 35%, #4a4a4a 0%, #050505 100%);
  border-radius: 20px;
  border: 2px solid #222;
  color: var(--neon);
  font-size: 30px;
  font-weight: bold;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 10px 20px rgba(0,0,0,0.5);
  cursor: pointer;
  user-select: none;
  transition: 0.1s;
  text-shadow: 0 0 10px var(--neon);
}

.lf-bnt:active, .lf-bnt.active {
  transform: scale(0.9);
  box-shadow: inset 0 5px 10px rgba(0,0,0,0.8), 0 0 20px var(--neon);
  border-color: var(--neon);
}

</style>
</head>
<body>

<div class="ps-controller">
  
  <!-- CẦN TRÁI -->
  <div class="bnt-zone">
    <div class="fb-button-container">
      <div id="btnF" class="fb-bnt">▲</div>
      <div id="btnB" class="fb-bnt">▼</div>
    </div>
  </div>

  <!-- BẢNG TRUNG TÂM -->
  <div class="touchpad">
    <div class="led-strip"></div>
    <h2>REMOTE CONTROLER</h2>
    

    <!-- THANH TRƯỢT TỐC ĐỘ BỔ SUNG -->
    <div class="speed-ctrl-container">
      <input type="range" id="speedSlider" class="speed-slider" min="0" max="100" value="0">
      <div id="speedValueDisplay">0</div>
    </div>
  </div>

  <!-- CỤM NÚT XOAY PHẢI -->
  <div class="bnt-zone">
    <div class="button-container">
      <div id="btnL" class="lf-bnt">◀</div>
      <div id="btnR" class="lf-bnt">▶</div>
    </div>
  </div>

</div>

<script>
// Cho phép slider hoạt động bình thường trên mobile
document.addEventListener('touchmove', e => { 
  if(e.target.id !== 'speedSlider') e.preventDefault(); 
}, {passive: false});

let vx=0, vy=0, spd=0;
let lastCommand = ""; // Biến lưu trữ lệnh cuối cùng để kiểm tra trùng lặp

// Xử lý slider
const slider = document.getElementById('speedSlider');
const speedDisplay = document.getElementById('speedValueDisplay');

slider.addEventListener('input', function() {
  spd = this.value;
  speedDisplay.innerText = spd;
  send();
});

function setupRotateButton(id, xValue) {
    const btn = document.getElementById(id);

    const start = (e) => {
        e.preventDefault();
        vx = xValue;   // -1 trái | +1 phải
        btn.classList.add('active');
        send();
    };

    const end = (e) => {
        e.preventDefault();
        vx = 0;
        btn.classList.remove('active');
        send();
    };

    btn.addEventListener('touchstart', start);
    btn.addEventListener('touchend', end);
    btn.addEventListener('mousedown', start);
    btn.addEventListener('mouseup', end);
    btn.addEventListener('mouseleave', end);
    btn.addEventListener('touchcancel', end);
}


function setupMoveButton(id, yValue) {
    const btn = document.getElementById(id);

    const start = (e) => {
        e.preventDefault();
        vy = yValue;   // +1 hoặc -1
        btn.classList.add('active');
        send();
    };

    const end = (e) => {
        e.preventDefault();
        vy = 0;
        btn.classList.remove('active');
        send();
    };

    btn.addEventListener('touchstart', start);
    btn.addEventListener('touchend', end);
    btn.addEventListener('mousedown', start);
    btn.addEventListener('mouseup', end);
    btn.addEventListener('mouseleave', end);
    btn.addEventListener('touchcancel', end);
}

const keyState = {
  ArrowUp: false,
  ArrowDown: false,
  ArrowLeft: false,
  ArrowRight: false
};

function updateFromKeyboard() {
  vy = 0;
  vx = 0;

  if (keyState.ArrowUp) vy = 1;
  else if (keyState.ArrowDown) vy = -1;

  if (keyState.ArrowLeft) vx = -1;
  else if (keyState.ArrowRight) vx = 1;

  send();
}

document.addEventListener('keydown', e => {
  if (e.key in keyState) {
    if (!keyState[e.key]) {
      keyState[e.key] = true;

      // 👉 bật hiệu ứng nút
      const btnId = keyToButton[e.key];
      if (btnId) {
        document.getElementById(btnId).classList.add('active');
      }

      updateFromKeyboard();
    }
    e.preventDefault();
  }
});

const keyToButton = {
  ArrowUp: 'btnF',
  ArrowDown: 'btnB',
  ArrowLeft: 'btnL',
  ArrowRight: 'btnR'
};


document.addEventListener('keyup', e => {
  if (e.key in keyState) {
    keyState[e.key] = false;

    // 👉 tắt hiệu ứng nút
    const btnId = keyToButton[e.key];
    if (btnId) {
      document.getElementById(btnId).classList.remove('active');
    }

    updateFromKeyboard();
    e.preventDefault();
  }
});



setupRotateButton('btnL', -1);
setupRotateButton('btnR', 1);
setupMoveButton('btnF', 1);   // ▲ Tiến
setupMoveButton('btnB', -1);  // ▼ Lùi


// HÀM GỬI DỮ LIỆU ĐÃ CẬP NHẬT THEO YÊU CẦU
function send() {
    let dir = "S"; // Mặc định là Dừng (Stop)

    if (vy == 1 && vx == 0) dir = "F";  // Tiến 
    else if (vy == -1 && vx == 0) dir = "B"; // Lùi 
    else if (vx == -1 && vy == 0) dir = "L"; // Trái 
    else if (vx == 1 && vy == 0) dir = "R"; // Phải 
    else if (vx == -1 && vy == 1) dir = "FL"; // Tiến trái 
    else if (vx == 1 && vy == 1) dir = "FR";  // Tiến phải 
    else if (vx == -1 && vy == -1) dir = "BL"; // Lùi trái
    else if (vx == 1 && vy == -1) dir = "BR";  // Lùi phải 

    // Tạo chuỗi lệnh hiện tại để so sánh
    let currentCommand = `${dir},${spd}`;
    
    // Kiểm tra nếu lệnh mới giống lệnh vừa gửi thì thoát không gửi nữa
    if (currentCommand === lastCommand) return;
    
    // Cập nhật lệnh cuối cùng
    lastCommand = currentCommand;

    // Gửi về server theo định dạng "HƯỚNG,TỐCĐỘ"
    fetch(`/control?v=${currentCommand}`).catch(() => {});
}
</script>
</body>
</html>
)rawliteral";