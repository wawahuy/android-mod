import { GLTF, GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';
import * as three from 'three';

let isDebugObject = true;
let debugObjects: three.Object3D[] = [];

export function loadGLTF(url: string) {
  return new Promise<GLTF>((resolve, reject) => {
    const loader = new GLTFLoader();
    loader.load(
      url,
      (m) => resolve(m),
      () => {},
      (err) => reject(err),
    );
  });
}

export function addDebug(...args: three.Object3D[]) {
  debugObjects = debugObjects.concat(args);
}

export function disableDebug() {
  debugObjects.forEach((obj) => {
    obj.userData['scene'] = obj.parent;
    obj.parent?.remove(obj);
  })
}

export function enableDebug() {
  debugObjects.forEach((obj) => {
    obj.userData['scene']?.add(obj);
    delete obj.userData['scene'];
  })
}

export function toggleDebug() {
  isDebugObject = !isDebugObject;
  if (isDebugObject) {
    disableDebug();
  } else {
    enableDebug();
  }
}