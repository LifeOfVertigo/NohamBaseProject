/**
 * Noham NEON Bridge - Communication with UE5 C++ Systems
 * Handles all communication between NEON HTML/JS UI and UE5 subsystems
 */

class NEONBridge {
    constructor() {
        this.isUE5Available = typeof ue !== 'undefined' && ue !== null;
        this.requestId = 0;
        this.pendingRequests = new Map();
        this.eventHandlers = new Map();
        
        // Initialize UE5 communication if available
        if (this.isUE5Available) {
            this.initializeUE5Communication();
        } else {
            console.warn('UE5 NEON API not available - running in standalone mode');
        }
    }

    /**
     * Initialize communication with UE5
     */
    initializeUE5Communication() {
        try {
            // Set up UE5 event listeners
            if (ue.interface && ue.interface.broadcast) {
                ue.interface.broadcast('neon_ui_ready', {
                    timestamp: Date.now(),
                    component: 'debug-overlay'
                });
            }
        } catch (error) {
            console.error('Failed to initialize UE5 communication:', error);
        }
    }

    /**
     * Generate unique request ID
     */
    generateRequestId() {
        return `req_${++this.requestId}_${Date.now()}`;
    }

    /**
     * Send command to UE5 with optional callback
     */
    async sendCommand(command, data = {}, callback = null) {
        const requestId = this.generateRequestId();
        
        const request = {
            requestId,
            command,
            data,
            timestamp: Date.now()
        };

        if (callback) {
            this.pendingRequests.set(requestId, callback);
        }

        if (this.isUE5Available) {
            try {
                if (ue.interface && ue.interface.broadcast) {
                    ue.interface.broadcast('neon_command', request);
                }
            } catch (error) {
                console.error('Failed to send command to UE5:', error);
                if (callback) {
                    callback({ error: error.message });
                    this.pendingRequests.delete(requestId);
                }
            }
        } else {
            // Simulate response in standalone mode
            setTimeout(() => {
                if (callback) {
                    callback(this.getSimulatedResponse(command, data));
                    this.pendingRequests.delete(requestId);
                }
            }, 100);
        }

        return requestId;
    }

    /**
     * Handle response from UE5
     */
    handleResponse(response) {
        const { requestId, data, error } = response;
        
        if (this.pendingRequests.has(requestId)) {
            const callback = this.pendingRequests.get(requestId);
            callback(error ? { error } : data);
            this.pendingRequests.delete(requestId);
        }
    }

    /**
     * Subscribe to events from UE5
     */
    addEventListener(eventType, handler) {
        if (!this.eventHandlers.has(eventType)) {
            this.eventHandlers.set(eventType, []);
        }
        this.eventHandlers.get(eventType).push(handler);
    }

    /**
     * Remove event listener
     */
    removeEventListener(eventType, handler) {
        if (this.eventHandlers.has(eventType)) {
            const handlers = this.eventHandlers.get(eventType);
            const index = handlers.indexOf(handler);
            if (index > -1) {
                handlers.splice(index, 1);
            }
        }
    }

    /**
     * Handle events from UE5
     */
    handleEvent(eventType, data) {
        if (this.eventHandlers.has(eventType)) {
            this.eventHandlers.get(eventType).forEach(handler => {
                try {
                    handler(data);
                } catch (error) {
                    console.error(`Error in event handler for ${eventType}:`, error);
                }
            });
        }
    }

    /**
     * Get simulated response for standalone testing
     */
    getSimulatedResponse(command, data) {
        switch (command) {
            case 'toggle_collision_visualization':
                return { success: true, enabled: !data.current };
            case 'toggle_ai_visualization':
                return { success: true, enabled: !data.current };
            case 'toggle_physics_visualization':
                return { success: true, enabled: !data.current };
            case 'toggle_rendering_visualization':
                return { success: true, enabled: !data.current };
            case 'toggle_debug_camera':
                return { success: true, enabled: !data.current };
            case 'execute_console_command':
                return { 
                    success: true, 
                    output: `Executed: ${data.command}\nResult: Command executed successfully`
                };
            case 'get_performance_stats':
                return {
                    fps: Math.floor(Math.random() * 20) + 100,
                    frameTime: (Math.random() * 5 + 8).toFixed(2),
                    memoryUsage: Math.floor(Math.random() * 500) + 1000
                };
            case 'inspect_object':
                return {
                    name: 'TestActor',
                    class: 'AActor',
                    location: { x: 100, y: 200, z: 300 },
                    properties: [
                        'ActorLocation: (100.0, 200.0, 300.0)',
                        'ActorRotation: (0.0, 0.0, 0.0)',
                        'ActorScale3D: (1.0, 1.0, 1.0)',
                        'bHidden: false'
                    ]
                };
            default:
                return { success: false, error: `Unknown command: ${command}` };
        }
    }

    // Debug Tools API Methods

    /**
     * Toggle collision visualization
     */
    async toggleCollisionVisualization(currentState, callback) {
        return this.sendCommand('toggle_collision_visualization', { current: currentState }, callback);
    }

    /**
     * Toggle AI visualization
     */
    async toggleAIVisualization(currentState, callback) {
        return this.sendCommand('toggle_ai_visualization', { current: currentState }, callback);
    }

    /**
     * Toggle physics visualization
     */
    async togglePhysicsVisualization(currentState, callback) {
        return this.sendCommand('toggle_physics_visualization', { current: currentState }, callback);
    }

    /**
     * Toggle rendering visualization
     */
    async toggleRenderingVisualization(currentState, callback) {
        return this.sendCommand('toggle_rendering_visualization', { current: currentState }, callback);
    }

    /**
     * Toggle debug camera
     */
    async toggleDebugCamera(currentState, callback) {
        return this.sendCommand('toggle_debug_camera', { current: currentState }, callback);
    }

    /**
     * Execute console command
     */
    async executeConsoleCommand(command, callback) {
        return this.sendCommand('execute_console_command', { command }, callback);
    }

    /**
     * Get performance statistics
     */
    async getPerformanceStats(callback) {
        return this.sendCommand('get_performance_stats', {}, callback);
    }

    /**
     * Inspect selected object
     */
    async inspectObject(callback) {
        return this.sendCommand('inspect_object', {}, callback);
    }

    /**
     * Get debug settings
     */
    async getDebugSettings(callback) {
        return this.sendCommand('get_debug_settings', {}, callback);
    }
}

// Global instance
window.neonBridge = new NEONBridge();

// Export for module systems
if (typeof module !== 'undefined' && module.exports) {
    module.exports = NEONBridge;
}