var __spreadArray = (this && this.__spreadArray) || function (to, from, pack) {
    if (pack || arguments.length === 2) for (var i = 0, l = from.length, ar; i < l; i++) {
        if (ar || !(i in from)) {
            if (!ar) ar = Array.prototype.slice.call(from, 0, i);
            ar[i] = from[i];
        }
    }
    return to.concat(ar || Array.prototype.slice.call(from));
};
define("index", ["require", "exports"], function (require, exports) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    exports.NEON_Bridge_Web = void 0;
    // Define the NEON API
    var NEON;
    (function (NEON) {
        function invokeUnrealEvent(delegate, data) {
            if (data === void 0) { data = {}; }
            return NEON_Bridge_Unreal.invokeUnrealEvent(delegate, data);
        }
        NEON.invokeUnrealEvent = invokeUnrealEvent;
        NEON.invokeUnreal = invokeUnrealEvent;
        function invokeUnrealFunction(delegate, data) {
            if (data === void 0) { data = {}; }
            return NEON_Bridge_Unreal.invokeUnrealFunction(delegate, data);
        }
        NEON.invokeUnrealFunction = invokeUnrealFunction;
        function onInvoke(delegate, callback) {
            NEON_Bridge_Web.registerCallback(delegate, callback);
        }
        NEON.onInvoke = onInvoke;
        function invoke(delegate, data) {
            NEON_Bridge_Web.invoke(delegate, data);
        }
        NEON.invoke = invoke;
        function setVerbose(verbose) {
            Log.setVerbose(verbose);
        }
        NEON.setVerbose = setVerbose;
    })(NEON || (NEON = {}));
    var Log = /** @class */ (function () {
        function Log() {
        }
        Log.setVerbose = function (verbose) {
            Log.verbose = verbose;
        };
        Log.info = function () {
            var args = [];
            for (var _i = 0; _i < arguments.length; _i++) {
                args[_i] = arguments[_i];
            }
            if (!Log.verbose)
                return;
            console.log.apply(console, __spreadArray(["[NEON]"], args, false));
        };
        Log.error = function () {
            var args = [];
            for (var _i = 0; _i < arguments.length; _i++) {
                args[_i] = arguments[_i];
            }
            console.error.apply(console, __spreadArray(["[NEON]"], args, false));
        };
        Log.verbose = false;
        return Log;
    }());
    var NEON_Bridge_Web = /** @class */ (function () {
        function NEON_Bridge_Web() {
        }
        NEON_Bridge_Web.registerCallback = function (id, callback) {
            Log.info('Registering NEON callback', id);
            NEON_Bridge_Web.callbacks[id] = callback;
        };
        NEON_Bridge_Web.invoke = function (id, dataRaw) {
            if (dataRaw === void 0) { dataRaw = '{}'; }
            var data;
            try {
                data = JSON.parse(dataRaw);
            }
            catch (e) {
                Log.error('Invoke NEON web callback failed: data is not JSON parseable: ', dataRaw);
                return;
            }
            if (!NEON_Bridge_Web.callbacks[id]) {
                Log.error("Invoke NEON web callback failed: callback not found: ".concat(id));
                return;
            }
            Log.info('Invoke NEON web callback', id, data);
            NEON_Bridge_Web.callbacks[id](data);
        };
        NEON_Bridge_Web.callbacks = {};
        return NEON_Bridge_Web;
    }());
    exports.NEON_Bridge_Web = NEON_Bridge_Web;
    // Define the NEON Bridge to be called from Web
    var NEON_Bridge_Unreal = /** @class */ (function () {
        function NEON_Bridge_Unreal() {
        }
        NEON_Bridge_Unreal.invokeUnreal = function (delegate, data) {
            return NEON_Bridge_Unreal.invokeUnrealEvent(delegate, data);
        };
        NEON_Bridge_Unreal.invokeUnrealFunction = function (delegate, data) {
            if (!delegate) {
                Log.error('NEON.invokeUnrealFunction failed: delegate is required');
                return Promise.reject({ errorCode: 101, errorMessage: 'Delegate is required' });
            }
            delegate = 'Invoke_' + delegate;
            Log.info('NEON.invokeUnrealFunction', delegate, data);
            return new Promise(function (resolve, reject) {
                if (!window.cefQuery) {
                    Log.error('NEON.invokeUnrealFunction failed: cefQuery is not defined');
                    return reject({ errorCode: 103, errorMessage: 'cefQuery is not defined' });
                }
                window.cefQuery({
                    request: JSON.stringify({
                        type: 'function',
                        delegate: delegate,
                        parameters: data
                    }),
                    onSuccess: function (response) {
                        Log.info("NEON.invokeUnrealFunction[".concat(delegate, "] succeeded: ").concat(response));
                        try {
                            var result = JSON.parse(response);
                            resolve(result);
                        }
                        catch (e) {
                            Log.error("NEON.invokeUnrealFunction[".concat(delegate, "] failed to parse response: ").concat(response));
                            reject({ errorCode: 102, errorMessage: 'Failed to parse response' });
                        }
                    },
                    onFailure: function (errorCode, errorMessage) {
                        Log.error("NEON.invokeUnrealFunction[".concat(delegate, "] failed: ").concat(errorCode, " - ").concat(errorMessage));
                        reject({ errorCode: errorCode, errorMessage: errorMessage });
                    }
                });
            });
        };
        NEON_Bridge_Unreal.invokeUnrealEvent = function (delegate, data) {
            if (data === void 0) { data = {}; }
            if (!delegate) {
                Log.error('NEON.invokeUnrealFunction failed: delegate is required');
                return Promise.reject({ errorCode: 101, errorMessage: 'Delegate is required' });
            }
            delegate = 'OnInvoke_' + delegate;
            Log.info('NEON.invokeUnrealEvent', delegate, data);
            return new Promise(function (resolve, reject) {
                if (!window.cefQuery) {
                    Log.error('NEON.invokeUnrealFunction failed: cefQuery is not defined');
                    return reject({ errorCode: 103, errorMessage: 'cefQuery is not defined' });
                }
                window.cefQuery({
                    request: JSON.stringify({
                        type: 'event',
                        delegate: delegate,
                        parameters: data
                    }),
                    onSuccess: function (response) {
                        Log.info("NEON.invokeUnrealEvent[".concat(delegate, "] succeeded."));
                        resolve(null);
                    },
                    onFailure: function (errorCode, errorMessage) {
                        Log.error("NEON.invokeUnrealEvent[".concat(delegate, "] failed: ").concat(errorCode, " - ").concat(errorMessage));
                        reject({ errorCode: errorCode, errorMessage: errorMessage });
                    }
                });
            });
        };
        return NEON_Bridge_Unreal;
    }());
    // Define the NEON Bridge to be called from Unreal
    window.NEON_Bridge_Web_Invoke = NEON.invoke;
    exports.default = NEON;
});
