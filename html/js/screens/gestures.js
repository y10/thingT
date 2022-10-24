import { jQuery, Router, Status, Http } from "../system";

const html = `
<style>
.container {
  width: 80vw;
  max-width: 300px;
}

#fu{
  background-color: var(--info-background-color);
  color: var(--warn-text-color);
}

#hm{
  background-color: var(--alert-background-color);
  color: var(--alert-text-color);
}

#v{
  background-color: var(--warn-background-color);
  color: var(--warn-text-color);
}

button {
  border: 0;
  border-radius: 0.3rem;
  background-color: var(--secondary-background-color);
  line-height: 2.4rem;
  font-size: 1.2rem;
  width: 100%;
  max-width: 300px;
  color: var(--secondary-text-color);
  margin: 8px 0;
}
</style>
<div class="container">
  <button id="h5">High 5</button>
  <button id="hm">Rock</button>
  <button id="fu">Welcome</button>
  <button id="v">Victory</button>
</div>
`;
export class Gestures extends HTMLElement {
  async connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(html, ($) => {
      $("#h5").on("click", (e) => this.onHiFive());
      $("#hm").on("click", (e) => this.onHeavyMetal());
      $("#fy").on("click", (e) => this.onFuckYou());
      $("#v").on("click", (e) => this.onVictory());
    });
  }

  async onHiFive() {
    await Http.json("GET", `api/servo/all/position?v=0`);
  }

  async onHeavyMetal() {
    await Http.json("GET", `api/servo/all/position?v=0`);
    await Http.json("GET", `api/servo/2/position?v=180`);
    await Http.json("GET", `api/servo/3/position?v=180`);
    await Http.json("GET", `api/servo/0/position?v=180`);
  }

  async onFuckYou() {
    await Http.json("GET", `api/servo/all/position?v=0`);
    await Http.json("GET", `api/servo/4/position?v=180`);
    await Http.json("GET", `api/servo/3/position?v=180`);
    await Http.json("GET", `api/servo/2/position?v=180`);
    await Http.json("GET", `api/servo/0/position?v=180`);
  }

  async onVictory() {
    await Http.json("GET", `api/servo/all/position?v=0`);
    await Http.json("GET", `api/servo/4/position?v=180`);
    await Http.json("GET", `api/servo/3/position?v=180`);
    await Http.json("GET", `api/servo/0/position?v=180`);
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }
}
