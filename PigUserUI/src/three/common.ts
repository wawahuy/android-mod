import { GLTF, GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';

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