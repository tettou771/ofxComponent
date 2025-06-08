# ofxComponent

ofxComponent は openFrameworks (oF) 用のアドオンです。
oF 内で UI を作成するための便利なツールが不足していたため、このアドオンが作成されました。

このツールの目的は、UI コンポーネントのオブジェクト階層（親子関係）の整理を簡素化することです。各オブジェクト内での相対的なマウス座標の使用や、相対座標系内でのオブジェクト位置関係の整理などが含まれます。

描画順序も考慮されており、親オブジェクトが子オブジェクトより先に描画されることが保証されているため、ボタンやビューを持つ UI パネルなどの構造を作成しやすくなっています。具体的には、setPos() メソッドを使用して相対位置を設定し、getPos() を使用して親に対する自分の座標を取得し、getMousePos() を使用して自分の座標系内での相対的なマウス座標を取得できます。

# 依存アドオン

- 他のアドオンには依存していません。

# テスト環境

- of0.10.1 以降

# 使用方法

まず、提供されているサンプルを参照してください。
このツールを使用するには、作成するすべてのオブジェクトを ofxComponentBase クラスから継承し、shared_ptr を使用して管理する必要があります。shared_ptr の機能に注意を払い、慎重に使用してください。setChild() メソッドを使用して親子関係を確立し、親と子オブジェクトをリンクします。親子関係は実行時に変更できます。

オブジェクト階層全体を管理するための ofxComponentManager クラスもあります。このクラスのインスタンスを 1 つだけ作成し、最上位の親として設定する必要があります：

```cpp
// ofApp.h ofApp
shared_ptr<ofxComponentManager> manager;
```
このようにインスタンスを定義し、その後：

```cpp
// ofApp.cpp ofApp::setup()

manager->setChild(yourComponent);
// ...その他の子要素...

manager->setup();
```

オブジェクトを削除する場合は、destroy() を呼び出すと、次の更新時に削除されます。すぐには削除されないため、必要に応じて isDestroyed() を使用してオブジェクトが削除されたかどうかを確認する必要があります。

ofxComponentBase クラス（提供されるヘッダーファイル）には、位置決め、スケーリング、回転、マウスとキーイベント、描画制約など、コンポーネントを管理および制御するためのさまざまなメソッドと属性が含まれています。また、親子関係の管理、座標位置の変換、コンポーネントの破棄処理のためのメソッドも含まれています。

ofxComponent の使用法と実装の詳細については、提供されている ofxComponentBase.h ヘッダーファイルを参照してください。

## Unity のような onUpdate, onDraw

これらのメソッドをオーバーライドすることで、イベントを処理できます。

- `onStart();` // オブジェクトが作成された後、最初の onUpdate() の前に一度だけ呼び出されます。
- `onUpdate();`
- `onDraw();`


## 入力ハンドラ

- `onKeyPressed(ofKeyEventArgs&);`
- `onKeyReleased(ofKeyEventArgs&);`
- `onMouseMoved(ofMouseEventArgs&);`
- `onMousePressed(ofMouseEventArgs&);`
- `onMouseDragged(ofMouseEventArgs&);`
- `onMouseReleased(ofMouseEventArgs&);`
- `onDragEvent(ofDragInfo&);`

### マウスイベントの詳細

#### onMousePressedOverComponent vs onMousePressed

- **`onMousePressed(ofMouseEventArgs&)`**: 他のコンポーネントが上に重なっていても、コンポーネントのどこかでマウスが押されたときに呼び出されます
- **`onMousePressedOverComponent(ofMouseEventArgs&)`**: そのコンポーネントがクリック位置で最上位（表示されている）コンポーネントの場合のみ呼び出されます

表示されている最上位レイヤーでのみクリックを登録したいボタンのようなインタラクションには、`onMousePressedOverComponent` を使用してください。

#### isMouseOver() メソッド

- **`isMouseOver()`**: マウスカーソルがこのコンポーネントの上にあり、かつこのコンポーネントがその位置で最上位にある場合に true を返します
- UI 要素でホバー状態を実装するのに便利です
- レイヤーチェックを自動的に処理 - 他のコンポーネントが上にあるかどうかを手動で確認する必要はありません

#### マウスイベントシステム統合

コンポーネントは、標準的なイベント処理のために組み込みの `ofEvent` インスタンスを提供します：

```cpp
// リッスンできる組み込みイベント:
ofEvent<void> mousePressedOverComponentEvents;  // 最上位コンポーネントに対してのみトリガー
ofEvent<ofMouseEventArgs> mousePressedEvents;   // カーソル下のどのコンポーネントに対してもトリガー
// ... その他のマウスイベント用の他のイベント
```

使用例:
```cpp
// ofApp の setup で:
ofAddListener(myButton->mousePressedOverComponentEvents, this, &ofApp::onButtonClicked);

// イベントハンドラメソッド:
void ofApp::onButtonClicked() {
    // ボタンクリックを処理
}
```

## タイマーシステム

ofxComponent は、遅延関数実行をスケジュールするための組み込みタイマーシステムを提供します。これは、アニメーション、UI トランジション、自動非表示機能、その他の時間ベースの動作に特に便利です。

### 基本的なタイマー使用法

#### addTimerFunction()

指定された遅延後に関数を実行するようにスケジュールします：

```cpp
// 基本的な使用法 - 2秒後に実行
auto timer = addTimerFunction([]() {
    ofLog() << "Timer executed!";
}, 2.0f);

// コンポーネントコンテキスト付き
auto timer = addTimerFunction([this]() {
    this->hide();  // 遅延後にこのコンポーネントを非表示
}, 1.5f);
```

タイマーは後でタイマーを制御するために使用できる `shared_ptr<Timer>` を返します。

### タイマー制御メソッド

#### setTimerPaused() - コンポーネントレベル制御

コンポーネントに関連付けられたすべてのタイマーを一時停止または再開します：

```cpp
setTimerPaused(true);   // このコンポーネントのすべてのタイマーを一時停止
setTimerPaused(false);  // このコンポーネントのすべてのタイマーを再開

// タイマーが一時停止されているかチェック
if (isTimerPaused()) {
    // コンポーネントタイマーが一時停止中
}
```

これは以下に自動的に適用されます：
- コンポーネント内のすべての既存タイマー
- すべての子コンポーネント（階層的一時停止）
- 一時停止中に作成された新しいタイマーは一時停止状態を継承

#### clearTimerFunctions() - すべてのタイマーをキャンセル

コンポーネント内のすべての保留中タイマーをキャンセルします：

```cpp
clearTimerFunctions();  // すべてのタイマーを安全にキャンセル
```

これは、特にコンポーネントが破棄またはリセットされるときに、すべてのタイマー活動を停止する推奨方法です。

### 個別タイマー制御

返されたタイマー参照を使用して個別タイマーを制御できます：

```cpp
// タイマー参照を保存
auto myTimer = addTimerFunction([this]() {
    // タイマー関数
}, 3.0f);

// 後で、個別タイマーを制御
myTimer->cancel();           // この特定のタイマーをキャンセル
myTimer->setPaused(true);    // この特定のタイマーを一時停止
myTimer->setPaused(false);   // この特定のタイマーを再開

// タイマー状態をチェック
if (myTimer->isPaused()) {
    // タイマーが一時停止中
}
if (myTimer->isDone()) {
    // タイマーが実行またはキャンセルされた
}
```

### タイマーシステムの機能

- **時間単位**: すべてのタイマー関数は秒を使用（ミリ秒ではありません）
- **自動クリーンアップ**: 実行およびキャンセルされたタイマーは自動的に削除
- **一時停止保持**: 一時停止されたタイマーが再開されるとき、残り時間を維持
- **階層制御**: 親コンポーネントの一時停止状態がすべての子に影響
- **メモリ安全**: メモリリークを防ぐために `shared_ptr` と `weak_ptr` を使用
- **スレッドセーフ**: シングルスレッドの openFrameworks 使用向けに設計

### 一般的なパターン

#### 自動非表示トーストメッセージ
```cpp
void showToast(const string& message, float duration = 2.0f) {
    isVisible = true;
    displayText = message;
    
    // 既存の非表示タイマーをキャンセル
    if (hideTimer) hideTimer->cancel();
    
    // 自動非表示をスケジュール
    hideTimer = addTimerFunction([this]() {
        isVisible = false;
    }, duration);
}
```

#### 順次アニメーションチェーン
```cpp
void startAnimationSequence() {
    // ステップ 1: フェードイン
    addTimerFunction([this]() {
        startFadeIn();
    }, 0.0f);
    
    // ステップ 2: 待機してから移動
    addTimerFunction([this]() {
        startMovement();
    }, 1.0f);
    
    // ステップ 3: 最終アクション
    addTimerFunction([this]() {
        onAnimationComplete();
    }, 3.0f);
}
```

#### 一時停止/再開制御
```cpp
void pauseAllActivity() {
    setTimerPaused(true);  // このコンポーネントと子のすべてのタイマーを一時停止
}

void resumeAllActivity() {
    setTimerPaused(false); // すべてのタイマーを再開、残り時間を維持
}

void stopAllActivity() {
    clearTimerFunctions();  // すべてのタイマーを永続的にキャンセル
}
```

### 実装例

以下を実演する完全な動作例については `exampleTimer` を参照してください：
- 自動非表示タイマー付きトーストメッセージ
- 一時停止/再開/停止制御付き順次メッセージチェーン
- タイマーキャンセルと管理
- コンポーネントイベントを使用したイベント駆動タイマー制御

## 位置、階層など

- `setPos(ofVec2f pos);` // オブジェクトの相対位置を設定
- `getPos();` // 親に対するオブジェクトの位置を取得
- `getMousePos();` // オブジェクトの座標系に対する相対的なマウス位置を取得
- `setChild(shared_ptr<ofxComponentBase> child);` // オブジェクト間の親子関係を確立
- `getChildren();` // 子オブジェクトのリストを取得
- `setActive(bool active);` // オブジェクトをアクティブまたは非アクティブに設定

# サンプル

このアドオンには、始めるのに役立ついくつかのサンプルが含まれています：

## 基本サンプル (`example/`)

基本的なコンポーネント使用法と階層のシンプルなデモンストレーション。

## タイマーサンプル (`exampleTimer/`)

以下の機能を含むタイマーシステムの包括的なデモンストレーション：
- 自動非表示機能付きトーストメッセージ
- 一時停止/再開/停止制御付き順次メッセージチェーン
- `mousePressedOverComponentEvents` を使用したイベント駆動インタラクション
- 適切なタイマー管理とキャンセル

このサンプルは、時間ベースの UI インタラクションとコンポーネントイベント処理を学ぶのに特に便利です。

# サンプル実装

## MyComponent サンプル

```cpp:MyComponent.h
#include "ofxComponent.h"
using namespace ofxComponent;

class MyComponent : public ofxComponentBase {
	void onStart() override {
		// コンポーネントサイズを設定
		setWidth(200);
		setHeight(100);

		// 相対位置を設定
		setPos(50, 50);
	}
	void onDraw() override {
		// 背景を描画
		ofSetColor(ofColor::yellow);
		// getWidth(), getHeight() はコンポーネントサイズを返します。
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
	void onMousePressedOverComponent(ofMouseEventArgs& mouse) override {
		ofLog() << "Clicked";
	}
};
```

## ofApp 統合

### ヘッダーファイル設定

ヘッダーをインクルードし、名前空間を使用します：

```cpp:ofApp.h
#include "ofxComponent.h"
using namespace ofxComponent;

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    // ... その他の標準 ofApp メソッド ...

private:
    // 重要: shared_ptr としてマネージャーを作成、getInstance() は使用しない
    shared_ptr<ofxComponentManager> manager;
    
    // あなたのコンポーネント
    shared_ptr<MyComponent> myComp;
};
```

### 実装設定

**重要**: 適切なコンポーネントシステム初期化のために、この正確な設定パターンに従ってください：

```cpp:ofApp.cpp
void ofApp::setup(){
    // 1. マネージャーを shared_ptr として作成
    manager = make_shared<ofxComponentManager>();
    
    // 2. マネージャーの描画エリアを設定（通常はフルウィンドウ）
    manager->setRect(0, 0, ofGetWidth(), ofGetHeight());
    
    // 3. コンポーネントを作成
    myComp = make_shared<MyComponent>();
    
    // 4. マネージャーにコンポーネントを追加（別のルートコンポーネントではなく）
    manager->addChild(myComp);
    
    // 5. 重要: すべてのコンポーネントを追加した後に setup() を呼び出す
    //    これにより、マネージャーのイベントリスナーが openFrameworks に登録されます
    manager->setup();
}

// 重要: これらのメソッドは空のままにしてください！
// マネージャーは setup() が呼び出された後、自動的にすべてのイベントを処理します
void ofApp::update() {
    // 空 - マネージャーが自動的に更新を処理
}

void ofApp::draw() {
    // 空 - マネージャーが自動的に描画を処理  
}

void ofApp::mousePressed(int x, int y, int button) {
    // 空 - マネージャーが自動的にマウスイベントを処理
}

// ... その他すべての ofApp イベントメソッドは空のままにできます
```

### ofxComponentManager の重要なポイント

1. **shared_ptr を使用**: 常に `make_shared<ofxComponentManager>()` として作成、`getInstance()` は使用しない
2. **描画エリアを設定**: `setRect()` を呼び出してマネージャーの座標空間を定義
3. **直接子を追加**: 別のルートコンポーネントではなく、マネージャーにコンポーネントを追加
4. **最後に setup() を呼び出す**: これにより openFrameworks に自動イベントリスナーが登録されます
5. **空の ofApp メソッド**: `manager->setup()` の後、手動でイベントを転送する必要はありません

### やってはいけないこと

```cpp
// ダメ: getInstance() パターンを使用
auto manager = ofxComponentManager::getInstance(); // これは存在しません

// ダメ: ofApp メソッドで手動でイベントを転送
void ofApp::update() {
    manager->update(); // 不要 - マネージャーが自動的に処理
}

// ダメ: 別のルートコンポーネントを作成
auto root = make_shared<ofxComponentBase>();
manager->addChild(root);
root->addChild(myComp); // 代わりに、マネージャーに直接追加
```

これは開発中のため、時々大きな変更がある可能性があります。

# 作者

http://github.com/tettou771

# ライセンス

MIT