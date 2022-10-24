import { Router } from "../system/router";
import { jQuery } from "../system/jquery";

const template = `<sketch-slider>
  <unit-test-servo index=0></unit-test-servo>
  <unit-test-servo index=1></unit-test-servo>
  <unit-test-servo index=2></unit-test-servo>
  <unit-test-servo index=3></unit-test-servo>
  <unit-test-servo index=4></unit-test-servo>
</sketch-slider>`;
export class UnitTestList extends HTMLElement {
  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(template, ($) => {
    });
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }
}
