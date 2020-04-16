try{
s_P("wkrYee");
var s_Iq=function(a){s_R.call(this,a.La);var b=this;this.wa=a.service.window.get();this.Ca=this.bB();this.Da=window.orientation;this.Aa=function(){var c=b.bB(),d="orientation"in window&&90===Math.abs(window.orientation)&&b.Da===-1*window.orientation;b.Da=window.orientation;if(c!==b.Ca||d){b.Ca=c;d=s_b(b.Ba);for(var e=d.next();!e.done;e=d.next()){e=e.value;var f=new s_M6a(c);try{e(f)}catch(g){s_5f(g)}}}};this.Ba=new Set;this.wa.addEventListener("resize",this.Aa);"orientation"in window&&this.wa.addEventListener("orientationchange",
this.Aa)};s_o(s_Iq,s_R);s_Iq.Ha=function(){return{service:{window:s_Wj}}};s_Iq.prototype.addListener=function(a){this.Ba.add(a)};s_Iq.prototype.removeListener=function(a){this.Ba.delete(a)};s_Iq.prototype.bB=function(){if(s_C7a()){var a=s_df(this.wa);a=new s_1e(a.width,Math.round(a.width*this.wa.innerHeight/this.wa.innerWidth))}else a=this.Wb()||(s_Bd()?s_C7a():this.wa.VisualViewport)?s_df(this.wa):new s_1e(this.wa.innerWidth,this.wa.innerHeight);return a.height<a.width};
s_Iq.prototype.destroy=function(){this.wa.removeEventListener("resize",this.Aa);this.wa.removeEventListener("orientationchange",this.Aa)};var s_C7a=function(){return s_Bd()&&s_1c()&&!navigator.userAgent.includes("GSA")};s_Iq.prototype.Wb=function(){return s_D7a};var s_D7a=!1;s_5i(s_Lva,s_Iq);

s_D7a=!0;

s_h().wa();

}catch(e){_DumpException(e)}
// Google Inc.
