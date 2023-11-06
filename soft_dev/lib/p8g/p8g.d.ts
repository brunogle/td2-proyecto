export default p8g;
export const CORNER: 0;
export const CORNERS: 1;
export const RADIUS: 2;
export const CENTER: 3;
export const RGB: 0;
export const HSB: 1;
export const HSL: 2;
export let width: number;
export let height: number;
export let keyCode: number;
export let keyIsPressed: boolean;
export let mouseX: number;
export let mouseY: number;
export let mouseButton: number;
export let mouseIsPressed: boolean;
export let deltaTime: number;
export const applyMatrix: (a: number, b: number, c: number, d: number, e: number, f: number) => void;
export const background: {
    (values: [number, number, number, number]): void;
    (gray: number): void;
    (gray: number, alpha: number): void;
    (v1: number, v2: number, v3: number): void;
    (v1: number, v2: number, v3: number, alpha: number): void;
};
export const colorMode: (mode: 0 | 1 | 2) => void;
export const ellipse: (x: number, y: number, w: number, h: number) => void;
export const ellipseMode: (mode: 0 | 1 | 2 | 3) => void;
export const fill: {
    (values: [number, number, number, number]): void;
    (gray: number): void;
    (gray: number, alpha: number): void;
    (v1: number, v2: number, v3: number): void;
    (v1: number, v2: number, v3: number, alpha: number): void;
};
export const image: {
    (img: Image, x: number, y: number): void;
    (img: Image, x: number, y: number, width: number): void;
    (img: Image, x: number, y: number, width: number, height: number): void;
    (img: Image, dx: number, dy: number, dw: number, dh: number, sx: number): void;
    (img: Image, dx: number, dy: number, dw: number, dh: number, sx: number, sy: number): void;
    (img: Image, dx: number, dy: number, dw: number, dh: number, sx: number, sy: number, sw: number): void;
    (img: Image, dx: number, dy: number, dw: number, dh: number, sx: number, sy: number, sw: number, sh: number): void;
};
export const imageMode: (mode: 0 | 1 | 2 | 3) => void;
export const line: (x1: number, y1: number, x2: number, y2: number) => void;
export const loadFont: (filename: string) => Promise<Font>;
export const loadImage: (filename: string) => Promise<Image>;
export const millis: () => number;
export const noFill: () => void;
export const noSmooth: () => void;
export const noStroke: () => void;
export const noTint: () => void;
export const point: (x: number, y: number) => void;
export const pop: () => void;
export const push: () => void;
export const random: {
    (): number;
    (max: number): number;
    (min: number, max: number): number;
};
export const randomSeed: (seed: number) => void;
export const rect: (x: number, y: number, w: number, h: number) => void;
export const rectMode: (mode: 0 | 1 | 2 | 3) => void;
export const resetMatrix: () => void;
export const rotate: (angle: number) => void;
export const createCanvas: (width: number, height: number) => HTMLCanvasElement;
export const scale: {
    (s: number): void;
    (x: number, y: number): void;
};
export const smooth: () => void;
export const stroke: {
    (values: [number, number, number, number]): void;
    (gray: number): void;
    (gray: number, alpha: number): void;
    (v1: number, v2: number, v3: number): void;
    (v1: number, v2: number, v3: number, alpha: number): void;
};
export const strokeWeight: (weight: number) => void;
export const text: (str: string, x: number, y: number) => void;
export const textFont: (font: Font) => void;
export const textSize: (size: number) => void;
export const tint: {
    (values: [number, number, number, number]): void;
    (gray: number): void;
    (gray: number, alpha: number): void;
    (v1: number, v2: number, v3: number): void;
    (v1: number, v2: number, v3: number, alpha: number): void;
};
export const translate: (x: number, y: number) => void;
export const triangle: (x1: number, y1: number, x2: number, y2: number, x3: number, y3: number) => void;
export type Image = {
    _index: number;
    width: number;
    height: number;
};
export type Font = {
    _index: number;
};
declare const p8g: {
    draw: () => void;
    keyPressed: () => void;
    keyReleased: () => void;
    mouseMoved: () => void;
    mousePressed: () => void;
    mouseReleased: () => void;
    mouseWheel: (deltaY: number) => void;
};
