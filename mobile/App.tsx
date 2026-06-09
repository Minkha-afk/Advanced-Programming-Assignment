import React, { useState } from 'react';
import {
  SafeAreaView,
  StatusBar,
  StyleSheet,
  Text,
  TouchableOpacity,
  View,
} from 'react-native';

type CounterButtonProps = {
  label: string;
  onPress: () => void;
  layout?: 'row' | 'wide';
  variant?: 'primary' | 'secondary';
  isDarkMode: boolean;
};

function CounterButton({
  label,
  onPress,
  layout = 'wide',
  variant = 'primary',
  isDarkMode,
}: CounterButtonProps) {
  const isSecondary = variant === 'secondary';

  return (
    <TouchableOpacity
      activeOpacity={0.82}
      accessibilityRole="button"
      onPress={onPress}
      style={[
        styles.button,
        layout === 'row' ? styles.rowButton : styles.wideButton,
        isSecondary ? styles.secondaryButton : styles.primaryButton,
        isSecondary && { borderColor: isDarkMode ? '#94a3b8' : '#334155' },
      ]}
    >
      <Text
        style={[
          styles.buttonText,
          isSecondary && { color: isDarkMode ? '#f8fafc' : '#111827' },
        ]}
      >
        {label}
      </Text>
    </TouchableOpacity>
  );
}

export default function App() {
  const [count, setCount] = useState(0);
  const [isDarkMode, setIsDarkMode] = useState(false);

  const handleIncrement = () => {
    setCount((currentCount) => currentCount + 1);
  };

  const handleDecrement = () => {
    setCount((currentCount) => {
      if (currentCount === 0) {
        return 0;
      }

      return currentCount - 1;
    });
  };

  const handleReset = () => {
    setCount(0);
  };

  const toggleTheme = () => {
    setIsDarkMode((currentMode) => !currentMode);
  };

  const screenStyles = {
    backgroundColor: isDarkMode ? '#111827' : '#ffffff',
  };

  const textStyles = {
    color: isDarkMode ? '#f9fafb' : '#111827',
  };

  const subtitleStyles = {
    color: isDarkMode ? '#cbd5e1' : '#475569',
  };

  return (
    <SafeAreaView style={[styles.safeArea, screenStyles]}>
      <StatusBar barStyle={isDarkMode ? 'light-content' : 'dark-content'} />
      <View style={[styles.container, screenStyles]}>
        <Text style={[styles.title, textStyles]}>Digital Counter</Text>
        <Text style={[styles.subtitle, subtitleStyles]}>
          {isDarkMode ? 'Dark Mode' : 'Light Mode'}
        </Text>

        <View style={styles.counterPanel}>
          <Text style={[styles.counterValue, textStyles]}>{count}</Text>
        </View>

        <View style={styles.buttonRow}>
          <CounterButton
            label="Decrement"
            onPress={handleDecrement}
            layout="row"
            isDarkMode={isDarkMode}
          />
          <CounterButton
            label="Increment"
            onPress={handleIncrement}
            layout="row"
            isDarkMode={isDarkMode}
          />
        </View>

        <CounterButton
          label="Reset"
          onPress={handleReset}
          variant="secondary"
          isDarkMode={isDarkMode}
        />
        <CounterButton
          label="Toggle Theme"
          onPress={toggleTheme}
          variant="secondary"
          isDarkMode={isDarkMode}
        />
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safeArea: {
    flex: 1,
  },
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    paddingHorizontal: 24,
  },
  title: {
    fontSize: 32,
    fontWeight: '700',
    marginBottom: 8,
    textAlign: 'center',
  },
  subtitle: {
    fontSize: 18,
    fontWeight: '500',
    marginBottom: 32,
    textAlign: 'center',
  },
  counterPanel: {
    alignItems: 'center',
    justifyContent: 'center',
    minHeight: 140,
    marginBottom: 32,
  },
  counterValue: {
    fontSize: 88,
    fontWeight: '800',
    lineHeight: 100,
  },
  buttonRow: {
    flexDirection: 'row',
    gap: 12,
    width: '100%',
    maxWidth: 360,
    marginBottom: 12,
  },
  button: {
    alignItems: 'center',
    justifyContent: 'center',
    minHeight: 52,
    borderRadius: 8,
    paddingHorizontal: 16,
    paddingVertical: 14,
    marginBottom: 12,
  },
  rowButton: {
    flex: 1,
  },
  wideButton: {
    width: '100%',
    maxWidth: 360,
  },
  primaryButton: {
    backgroundColor: '#2563eb',
  },
  secondaryButton: {
    borderWidth: 1,
    backgroundColor: 'transparent',
  },
  buttonText: {
    color: '#ffffff',
    fontSize: 16,
    fontWeight: '700',
    textAlign: 'center',
  },
});
