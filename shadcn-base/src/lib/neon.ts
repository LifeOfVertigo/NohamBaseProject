// NEON Bridge Helper Library

// Wait for NEON to be available
export function waitForNEON(): Promise<void> {
  return new Promise((resolve) => {
    if (typeof (window as any).NEON !== 'undefined') {
      resolve();
      return;
    }

    // Poll for NEON availability
    const checkInterval = setInterval(() => {
      if (typeof (window as any).NEON !== 'undefined') {
        clearInterval(checkInterval);
        resolve();
      }
    }, 100);

    // Timeout after 5 seconds
    setTimeout(() => {
      clearInterval(checkInterval);
      console.error('[NEON] Failed to initialize after 5 seconds');
      resolve(); // Resolve anyway to prevent hanging
    }, 5000);
  });
}

// Call NEON event (one-way communication to UE5)
export function invokeUnrealEvent(eventName: string, params: any = {}) {
  if (typeof (window as any).NEON !== 'undefined') {
    (window as any).NEON.invokeUnrealEvent(eventName, params);
    console.log(`[NEON] ✓ Event sent: ${eventName}`, params);
  } else {
    console.error(`[NEON] ✗ Failed to send event: ${eventName} - NEON not available`);
    console.error('[NEON] Ensure widget parent class is UNEONWidget');
  }
}

// Call NEON function (two-way communication with return value)
export async function invokeUnrealFunction(functionName: string, params: any[] = []): Promise<any> {
  if (typeof (window as any).NEON !== 'undefined') {
    const result = await (window as any).NEON.invokeUnrealFunction(functionName, params);
    console.log(`[NEON] ✓ Function returned: ${functionName}`, result);
    return result;
  } else {
    console.error(`[NEON] ✗ Failed to call function: ${functionName} - NEON not available`);
    throw new Error('NEON not available');
  }
}
