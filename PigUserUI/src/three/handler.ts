import gsap from 'gsap';
import ScrollTrigger from 'gsap/ScrollTrigger';
import * as three from 'three'

export class AnimationRoundHandler {
  private _renderer: three.WebGLRenderer;
  private _scene: three.Scene;
  private _camera: three.Camera;
  private _cube: three.Group;
  private _isRunning = true;

  private _clock = new three.Clock();
  private _delta = 0;
  private _interval = 1 / 30;

  constructor(private readonly container: HTMLElement) {
    const width = window.innerWidth, height = window.innerHeight;
    this._camera = new three.PerspectiveCamera( 70, width / height, 0.01, 10 );
    this._scene = new three.Scene();

    const light = new three.AmbientLight(0xffffff, 0.05);
    this._scene.add(light);

    const dl = new three.PointLight( 0xffffff, 20, 100 );
    dl.position.set(-8, 8, 8);
    this._scene.add(dl);
  
    this._cube = this.buildRound();
    this._scene.add(this._cube);
    this._camera.position.z = 5;
    this._camera.position.x = 1;
    this._camera.position.y = 1;

    this._renderer = new three.WebGLRenderer( { antialias: true } );
    this._renderer.setSize( width, height );
    this._renderer.setClearColor( 0x000000, 0 );
    container.appendChild(this._renderer.domElement);

    this.onAnimation();
  }

  initGsap(id: string) {
    gsap.registerPlugin(ScrollTrigger);
    ScrollTrigger.defaults({
      immediateRender: false,
      scrub: true,
    });
    const tl = gsap.timeline();
    tl.to(this._cube.rotation, { z: 10, y: 5, scrollTrigger: {
      trigger: id,
      start: 'top bottom',
      end: 'top top',
    }});
  }

  release() {
    this._isRunning = false;
    this.container.removeChild(this._renderer.domElement);
  }

  onAnimation = () => {
    if (this._isRunning) {
      requestAnimationFrame(this.onAnimation);
    }

    this._delta += this._clock.getDelta();
    if (this._delta  > this._interval) {
        this.render();
        this._delta = this._delta % this._interval;
    }
  }

  private render() {
	  this._renderer.render(this._scene, this._camera);
  }

  private buildRound() {
    const group = new three.Group();

    const geometry = new three.BoxGeometry( 1, 1, 1 );
    const material = new three.MeshPhongMaterial( { color: 0x00ff00 } );
    const cube = new three.Mesh(geometry, material);
    cube.matrix.makeTranslation(1, 1, 1);
    cube.matrixAutoUpdate = false;

    group.add(cube);
    return group;
  }
}