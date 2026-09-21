import React, { useState, useRef, useEffect } from 'react';
import { View, Text, StyleSheet, Pressable, TextInput, Alert, Keyboard } from 'react-native';

// Gửi lại lệnh mỗi khoảng này trong lúc giữ nút, để bù các gói bị rớt trên Wi-Fi.
// Phải nhỏ hơn nhiều so với ARM_COMMAND_TIMEOUT_MS bên firmware (400ms) để watchdog không kích hoạt khi vẫn đang giữ.
const HOLD_REPEAT_MS = 150;

// ================= CẤU HÌNH GỬI LỆNH WI-FI =================
const sendCommandToESP = async (ip: string, command: string) => {
  if (!ip) return;
  try {
    await fetch(`http://${ip}/${command}`, { method: 'GET' });
    console.log("Đã gửi:", command);
  } catch (error: any) {
    console.log("Lỗi gửi tín hiệu:", error.message);
  }
};

// ================= KHAI BÁO KIỂU CHO NÚT ĐIỀU KHIỂN =================
interface ControlButtonProps {
  label: string;
  command: string;
  releaseCommand?: string;
  ip: string;
  isConnected: boolean;
  color?: string;
  width?: number;
  height?: number;
  borderRadius?: number;
}

const ControlButton: React.FC<ControlButtonProps> = ({
  label, command, releaseCommand = '0', ip, isConnected, color = '#444', width = 70, height = 70, borderRadius = 15
}) => {
  const repeatRef = useRef<ReturnType<typeof setInterval> | null>(null);

  const stopRepeating = () => {
    if (repeatRef.current) {
      clearInterval(repeatRef.current);
      repeatRef.current = null;
    }
  };

  const handlePressIn = () => {
    if (!isConnected) return;
    sendCommandToESP(ip, command);
    stopRepeating();
    repeatRef.current = setInterval(() => sendCommandToESP(ip, command), HOLD_REPEAT_MS);
  };

  const handlePressOut = () => {
    stopRepeating();
    if (!isConnected) return;
    sendCommandToESP(ip, releaseCommand);
  };

  useEffect(() => stopRepeating, []);

  return (
    <Pressable
      onPressIn={handlePressIn}
      onPressOut={handlePressOut}
      style={({ pressed }) => [
        styles.btn,
        { backgroundColor: pressed ? '#777' : color, width, height, borderRadius }
      ]}
    >
      <Text style={styles.btnText}>{label}</Text>
    </Pressable>
  );
};

// ================= KHAI BÁO KIỂU CHO NÚT TỐC ĐỘ =================
interface SpeedButtonProps {
  label: string;
  speedValue: number;
  activeColor: string;
  currentSpeed: number;
  setSpeed: (val: number) => void;
  ip: string;
  isConnected: boolean;
}

const SpeedButton: React.FC<SpeedButtonProps> = ({ 
  label, speedValue, activeColor, currentSpeed, setSpeed, ip, isConnected 
}) => {
  const isActive = currentSpeed === speedValue;

  return (
    <Pressable
      onPress={() => {
        if (!isConnected) return;
        setSpeed(speedValue);
        sendCommandToESP(ip, speedValue.toString());
      }}
      style={({ pressed }) => [
        styles.btn,
        { 
          width: 110, height: 30, borderRadius: 5, // ĐÃ GIẢM CHIỀU CAO VÀ CHIỀU RỘNG Ở ĐÂY
          backgroundColor: isActive ? activeColor : (pressed ? '#444' : '#222'),
          borderColor: isActive ? activeColor : '#555',
          opacity: isActive ? 1 : 0.5 
        }
      ]}
    >
      <Text style={[styles.btnText, { color: isActive ? '#fff' : '#888' }]}>{label}</Text>
    </Pressable>
  );
};

// ================= MÀN HÌNH CHÍNH =================
export default function App() {
  const [ipAddress, setIpAddress] = useState<string>('');
  const [isWifiConnected, setIsWifiConnected] = useState<boolean>(false);
  const [activeSpeed, setActiveSpeed] = useState<number>(1);
  
  const handleConnect = () => {
    if (ipAddress.trim() === '') {
      Alert.alert('Khoan đã', 'Ông chưa nhập địa chỉ IP của xe!');
      return;
    }
    Keyboard.dismiss();
    setIsWifiConnected(!isWifiConnected);
    if (!isWifiConnected) {
        Alert.alert('Đã chốt IP', 'Bây giờ xe đã sẵn sàng nhận lệnh!');
    }
  };

  return (
    <View style={styles.container}>
      
      {/* ================= CỘT TRÁI ================= */}
      <View style={styles.sidePanel}>
        <View style={styles.topControlGroup}>
            <View style={styles.row}>
                <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Cùi Ra (T)" command="T" releaseCommand="s" width={60} height={60} />
                <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="GẮP (J)" command="J" releaseCommand="s" color="#b30000" width={60} height={60} />
            </View>
            <View style={styles.row}>
                <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Cùi Vào (G)" command="G" releaseCommand="s" width={60} height={60} />
                <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="NHẢ (K)" command="K" releaseCommand="s" color="#004080" width={60} height={60} />
            </View>
        </View>

        {/* Cụm Lái Xe */}
        <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Tiến (F)" command="F" />
        <View style={styles.row}>
          <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Trái (L)" command="L" />
          <View style={styles.spacer} />
          <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Phải (R)" command="R" />
        </View>
        <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Lùi (B)" command="B" />
      </View>

      {/* ================= CỘT GIỮA ================= */}
      <View style={styles.centerPanel}>
        
        {/* Khung Nhập IP & Kết Nối */}
        <View style={styles.topStatus}>
            <TextInput 
              style={styles.ipInput} 
              placeholder="Nhập IP" 
              placeholderTextColor="#888"
              value={ipAddress}
              onChangeText={setIpAddress}
              editable={!isWifiConnected}
            />
            <Pressable 
              style={({ pressed }) => [
                styles.btnBluetooth, 
                { 
                  backgroundColor: isWifiConnected ? (pressed ? '#21867a' : '#2a9d8f') : (pressed ? '#0044cc' : '#0055ff'),
                  borderColor: isWifiConnected ? '#45c4b5' : '#88aaff'
                }
              ]}
              onPress={handleConnect}
            >
                <Text style={styles.btnBluetoothText}>
                  {isWifiConnected ? 'ĐÃ KẾT NỐI WI-FI 🟢' : 'KẾT NỐI XE 📡'}
                </Text>
            </Pressable>
        </View>
        
        {/* Màn hình hiển thị */}
        <View style={styles.screen}>
            <Text style={styles.safeLight}>{isWifiConnected ? '🟢 MẠNG TỐT' : '🔴'}</Text> 
            <Text style={styles.speedIndicator}>{activeSpeed}</Text>
            
            <Text style={styles.title}>DISTANCE</Text>
            <Text style={styles.distanceText}>30 cm</Text>
        </View>

        {/* Thanh chỉnh tốc độ */}
        <View style={styles.speedContainer}>
            <Text style={styles.speedTitle}>Tốc độ di chuyển</Text>
            <SpeedButton ip={ipAddress} isConnected={isWifiConnected} label="4" speedValue={4} activeColor="#e63946" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton ip={ipAddress} isConnected={isWifiConnected} label="3" speedValue={3} activeColor="#f4a261" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton ip={ipAddress} isConnected={isWifiConnected} label="2" speedValue={2} activeColor="#e9c46a" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
            <SpeedButton ip={ipAddress} isConnected={isWifiConnected} label="1" speedValue={1} activeColor="#2a9d8f" currentSpeed={activeSpeed} setSpeed={setActiveSpeed} />
        </View>
      </View>

      {/* ================= CỘT PHẢI ================= */}
      <View style={styles.sidePanel}>
        
        <View style={styles.topControlGroup}>
            <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="CÒI (V)" command="V" releaseCommand="v" color="#cccc00" width={75} height={75} borderRadius={40} />
        </View>

        {/* Nút Đỗ xe (P) - Đặt absolute lệch xuống dưới và sang phải */}
            <View style={{ position: 'absolute', top: 90, right: 50 }}>
                <ControlButton 
                  ip={ipAddress} 
                  isConnected={isWifiConnected} 
                  label="P" 
                  command="P" 
                  releaseCommand="P" 
                  color="#e63946" 
                  width={60} 
                  height={60}
                  borderRadius={30} 
                />
            </View>

        {/* Cụm Vai & Đế */}
        <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Vai Lên (W)" command="W" releaseCommand="s" />
        <View style={styles.row}>
          <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Đế Trái (A)" command="A" releaseCommand="s" />
          <View style={styles.spacer} />
          <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Đế Phải (D)" command="D" releaseCommand="s" />
        </View>
        <ControlButton ip={ipAddress} isConnected={isWifiConnected} label="Vai Xuống (X)" command="X" releaseCommand="s" />
      </View>

    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, flexDirection: 'row', backgroundColor: '#1e1e1e' },
  sidePanel: { flex: 1, justifyContent: 'flex-end', alignItems: 'center', paddingBottom: 10 },
  centerPanel: { flex: 0.8, justifyContent: 'space-between', alignItems: 'center', paddingVertical: 15, borderLeftWidth: 1, borderRightWidth: 1, borderColor: '#333' },
  topControlGroup: { position: 'absolute', top: 20, alignItems: 'center' },
  topStatus: { alignItems: 'center', marginBottom: 5 },
  ipInput: {
    backgroundColor: '#333', color: '#fff', width: 200, height: 40,
    borderRadius: 8, paddingHorizontal: 15, marginBottom: 10,
    borderWidth: 1, borderColor: '#555', textAlign: 'center'
  },
  btnBluetooth: { paddingHorizontal: 20, paddingVertical: 10, borderRadius: 25, borderWidth: 1, elevation: 3 },
  btnBluetoothText: { color: '#ffffff', fontWeight: 'bold', fontSize: 12, letterSpacing: 1 },
  screen: { 
    width: 220, height: 110, justifyContent: 'center', alignItems: 'center', 
    backgroundColor: '#0a0a0a', paddingVertical: 15, paddingHorizontal: 40,
    borderRadius: 8, borderWidth: 2, borderColor: '#444' 
  },
  safeLight: { position: 'absolute', top: 10, left: 10, fontSize: 10, color: '#aaa' },
  speedIndicator: { position: 'absolute', top: 10, right: 10, color: '#ffcc00', fontSize: 10, fontWeight: 'bold' },
  title: { color: '#888', fontSize: 12, letterSpacing: 2, marginBottom: 5 },
  distanceText: { color: '#00ffcc', fontSize: 45, fontWeight: 'bold' },
  speedContainer: { alignItems: 'center' },
  speedTitle: { color: '#aaa', fontSize: 13, marginBottom: 8 },
  row: { flexDirection: 'row', alignItems: 'center' },
  spacer: { width: 70, height: 70 }, 
  btn: { justifyContent: 'center', alignItems: 'center', margin: 3, borderWidth: 1, borderColor: '#555' },
  btnText: { color: '#fff', fontSize: 12, fontWeight: 'bold', textAlign: 'center' }
});